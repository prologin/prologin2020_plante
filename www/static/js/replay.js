$(function () {

    let $replay = $('#replay'),
        $playPause = $('#replay-playpause'),
        $previous = $('#replay-previous'),
        $next = $('#replay-next'),
        $turnLabel = $('#replay-turn-label'),
        $turnSlider = $('#replay-turn-slider');

    $replay.hide();

    $.when(
        $.getScript('/static/js/viewer.js')
    ).done(function() {
    });

});
