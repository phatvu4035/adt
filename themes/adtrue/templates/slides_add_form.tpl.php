<?php
/**
 * Created by PhpStorm.
 * User: tuanna0703
 * Date: 1/12/2017
 * Time: 10:02 AM
 */
//krumo($form);
?>
<div class="col-sm-6">
    <div class="panel panel-default">
        <div class="panel-heading">Thông tin chung</div>
        <div class="panel-body">
            <div class="form-group">
                <label>Tiêu đề Nội dung</label>
                <?php print drupal_render($form['title']); ?>
            </div>
            <div class="form-group">
                <label>Mô tả</label>
                <?php print drupal_render($form['body']); ?>
            </div>

        </div>
    </div>
</div>
<div class="col-sm-6">
    <div class="panel panel-default">
        <div class="panel-heading">Thông số</div>
        <div class="panel-body">
            <div class="form-group">
                <label>File Nội Dung</label>
                <?php print drupal_render($form['field_slide_file']); ?>
            </div>
            <div class="form-group">
                <label>File Phụ Đề (Nếu có)</label>
                <?php print drupal_render($form['field_slide_subtitle_url']); ?>
            </div>
            <div class="form-group col-sm-4">
                <label>Kiểu File</label>
                <?php print drupal_render($form['field_media_type']); ?>
            </div>
            <div class="form-group col-sm-4">
                <label>Thời lượng</label>
                <?php print drupal_render($form['field_file_time']); ?>
            </div>
            <div class="form-group col-sm-4">
                <label>Dung lượng</label>
                <?php print drupal_render($form['field_file_size']); ?>
            </div>
        </div>
    </div>
</div>



<?php
unset($form['field_slide_start_time']);
unset($form['field_slide_duration']);
unset($form['field_slide_status']);
unset($form['field_slide_order']);
unset($form['field_slide_review_02_status']);
unset($form['field_slide_review_01_status']);
?>
<div class="col-sm-12" style="margin-top:20px;">
    <?php print drupal_render_children($form);?>
</div>

