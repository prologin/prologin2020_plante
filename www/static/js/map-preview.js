$(function () {

    let $preview = $('#map-preview');

    $preview.hide();

    $.getScript('/static/js/pixi.min.js')
    .done(function() {
        $.getScript('/static/js/viewer.js')
        .done(function() {
            start_preview($replay);
            // reveal the UI
            $preview.fadeIn('fast');
        });
    });

});
