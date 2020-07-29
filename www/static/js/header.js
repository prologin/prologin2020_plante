// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright 2018 Alexandre Macabies

$(function() {
    $('[role=complementary] h1').text('').html('<object data="/static/img/banner.jpg" style="width:100%; margin-top: 0.5em;" type="image/jpeg" />');

    // let's have some easter egg
    const p1 = $('<img src="/static/img/rainbow.gif" class="easter-egg" />'),
        p2 = $('<img src="/static/img/pattern.gif" class="easter-egg" />');
    $('body').append(p1).append(p2);
    $('.easter-egg').on('click', function() {
        const p = $(this);
        if (p.hasClass('visible')) {
            p.removeClass('visible');
        } else {
            p.removeClass('peak').addClass('visible');
        }
    });
    [p1, p2].forEach(function (p) {
        if (Math.random() < .5) {
            p.css('left', 64 + Math.random() * (document.body.clientWidth - 128 - 64));
            setTimeout(function() {
                const p = Math.random() < .5 ? p1 : p2;
                p.addClass('peak');
            },
                1000 * (5 + Math.random() * 10));
        }
    });
});
