(function ($) {
    Drupal.behaviors.adtrue = {
        attach: function (context, settings) {
            $("a.add-playlist").click(function () {
                event.preventDefault();
                var broadcast_id = ($(this).data('broadcast'));
                var playlist_id = ($(this).data('playlist'));


                $.ajax({
                    url: Drupal.settings.basePath + 'broadcast/add/playlist/ajax',
                    type: 'post',
                    data: 'broadcast_id=' + broadcast_id + '&playlist_id=' + playlist_id,
                    dataType: 'json',
                    success: function (data) {
                        if (data.success === true) {
                            $("#tools_broadcast_"+playlist_id).hide();
                            $("#tools_broadcast_remove_"+playlist_id).show();
                        } else {
                            alert('Có lỗi trong quá trình xử lý dữ liệu!');
                        }
                    }
                });

            });

            $("a.remove-playlist").click(function () {
                event.preventDefault();

                var broadcast_id = ($(this).data('broadcast'));
                var playlist_id = ($(this).data('playlist'));

                $.ajax({
                    url: Drupal.settings.basePath + 'broadcast/remove/playlist/ajax',
                    type: 'post',
                    data: 'broadcast_id=' + broadcast_id + '&playlist_id=' + playlist_id,
                    dataType: 'json',
                    success: function (data) {
                        if (data.success === true) {
                            $("#tools_broadcast_"+playlist_id).show();
                            $("#tools_broadcast_remove_"+playlist_id).hide();
                        } else {
                            alert('Có lỗi trong quá trình xử lý dữ liệu!');
                        }
                    }
                });

            });
        }
    };
})(jQuery);