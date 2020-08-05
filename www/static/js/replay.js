$(function () {

    let $replay = $('#replay'),
        $playPause = $('#replay-playpause'),
        $previous = $('#replay-previous'),
        $next = $('#replay-next'),
        $turnLabel = $('#replay-turn-label'),
        $turnSlider = $('#replay-turn-slider');

    $replay.append('<div id="replay_view"></div>');
    let $replay_view = $("#replay_view");


    $replay_view.hide();

    $.getScript('/static/js/pixi.min.js')
    .done(function() {
        $.getScript('/static/js/viewer.js')
        .done(function() {
            console.log('finish loading');
            start_viewer($replay_view, $turnSlider);
            // reveal the UI
            $replay_view.find('canvas').css({
                'display': 'block',
                'max-width': '100%',
                'max-height': '100%',
                'margin': 'auto',
            });
            $replay_view.fadeIn('fast');

            $turnSlider.change(function () {
                let turnIndex = parseInt($turnSlider.val());
                $previous.prop('disabled', turnIndex <= 0);
                $next.prop('disabled', turnIndex > 100);
                $turnLabel.text(turnIndex);
            });
        });
    });

});
