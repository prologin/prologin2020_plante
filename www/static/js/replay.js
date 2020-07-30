$(function () {

    let $replay = $('#replay'),
        $playPause = $('#replay-playpause'),
        $previous = $('#replay-previous'),
        $next = $('#replay-next'),
        $turnLabel = $('#replay-turn-label'),
        $turnSlider = $('#replay-turn-slider');

    $replay.hide();

    $.getScript('/static/js/pixi.min.js')
    .done(function() {
        $.getScript('/static/js/viewer.js')
        .done(function() {
            console.log('finish loading');
            start_viewer($replay);
            // reveal the UI
            $replay.fadeIn('fast');
        });
    });

});
