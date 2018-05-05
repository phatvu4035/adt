/**
 * Created by tuanna0703 on 12/2/2017.
 */
(function ($) {
    Drupal.behaviors.adtrue = {
        attach: function (context, settings) {
            var video = videojs('player').ready(function(){
                var player = this;

                player.on('ended', function() {
                    alert('video is done!');
                });
            });
        }
    }
});
