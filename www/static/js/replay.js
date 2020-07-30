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
            start_viewer($replay, $turnSlider);
            // reveal the UI
            $replay.fadeIn('fast');

            $turnSlider.change(function () {
                let turnIndex = parseInt($turnSlider.val());
                $previous.prop('disabled', turnIndex <= 0);
                $next.prop('disabled', turnIndex > 100);
                $turnLabel.text(turnIndex);
            });
        });
    });

});
