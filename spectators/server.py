import asyncio
import functools
from http import HTTPStatus
import logging
import json
import mimetypes
import os.path
import websockets
import threading

import api
from state import Reader

logger = logging.Logger(__name__)


# from https://gist.github.com/artizirk/04eb23d957d7916c01ca632bb27d5436
async def process_request(sever_root, path, request_headers):
    if 'Upgrade' in request_headers:
        return # Probably a WebSocket connection

    response_headers = [
        ('Server', 'asyncio websocket server'),
        ('Connection', 'close'),
    ]

    if '?' in path:
        path = path[:path.find('?')]
    if not path.startswith('/www/'):
        path = '/www' + path

    # Derive full system path
    full_path = os.path.realpath(os.path.join(sever_root, path[1:]))

    # Validate the path
    if os.path.commonpath((sever_root, full_path)) != sever_root or \
            not os.path.exists(full_path) or not os.path.isfile(full_path):
        print('HTTP GET {} 404 NOT FOUND'.format(path))
        return HTTPStatus.NOT_FOUND, [], b'404 NOT FOUND'

    # Guess file content type
    mime_type = mimetypes.MimeTypes().guess_type(full_path)[0]
    response_headers.append(('Content-Type', mime_type))

    # Read the whole file into memory and send it out
    body = open(full_path, 'rb').read()
    response_headers.append(('Content-Length', str(len(body))))
    print('HTTP GET {} 200 OK'.format(path))
    return HTTPStatus.OK, response_headers, body

class Server:
    def __init__(self, state_reader: Reader, tv_mode=False):
        self.state_reader = state_reader
        self.tv_mode = tv_mode

        self.loop = asyncio.new_event_loop()

        self.run_thread = threading.Thread(target=self.run)

    def run(self):
        handler = functools.partial(process_request, os.getcwd())
        start_server = websockets.serve(self.handle_request, '127.0.0.1', 8765, process_request=handler, loop=self.loop)

        url = 'http://127.0.0.1:8765/www/index.html'
        for prog in ('chromium', 'google-chrome', 'google-chrome-stable', 'chromium-browser'):
            try:
                browser = asyncio.create_subprocess_exec(prog, '--app=' + url,
                                                         loop=self.loop)
                logging.info("Opening %s with %s", url, prog)
                self.loop.run_until_complete(browser)
                break
            except FileNotFoundError:
                pass

        self.loop.run_until_complete(start_server)
        self.loop.run_forever()

    def start(self):
        self.run_thread.start()

    async def send(self, websocket, cmd, **kwargs):
        msg = {}
        msg.update(kwargs)
        msg['c'] = cmd
        await websocket.send(json.dumps(msg))

    async def handle_message(self, msg, websocket):
        data = json.loads(msg)
        cmd = data['c']
        logger.debug(f'received WS message: {data}')
        if cmd == 'hello':
            await self.send(websocket,
                            'whatsup',
                            size=api.TAILLE_GRILLE,
                            canSeek=self.state_reader.can_seek(),
                            canGoBackwards=self.state_reader.can_go_backwards(),
                            tvMode=self.tv_mode)
            await self.go_next(websocket)
            return
        if cmd == 'next':
            await self.go_next(websocket)
            return
        if cmd == 'previous':
            await self.go_previous(websocket)
            return
        if cmd == 'seek':
            await self.go_seek(websocket, data['index'])
            return

    async def go_next(self, websocket):
        self.state = self.state_reader.get_next_state()
        if self.state:
            await self.send(websocket, 'turn', state=self.state)
            self.state_reader.go_next()
        if self.tv_mode and self.state_reader.is_ended():
            self.end_game()

    async def go_previous(self, websocket):
        self.state_reader.go_previous()
        self.state = self.state_reader.get_next_state()
        if self.state:
            await self.send(websocket, 'turn', state=self.state)

    async def go_seek(self, websocket, index):
        self.state_reader.go_seek(index)
        self.state = self.state_reader.get_next_state()
        if self.state:
            await self.send(websocket, 'turn', state=self.state)

    async def handle_request(self, websocket, path):
        running = True
        while running:
            try:
                await self.handle_message(await websocket.recv(), websocket)
            except websockets.exceptions.ConnectionClosedError:
                running = False

