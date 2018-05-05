<?php
function danhsach_noidung_video($form,&$form_state) {
	global $user,$base_url;

	$form = array();
	$header = array(
		array('data' => 'STT'),
        array('data' => 'Tên nội dung'),
        array('data' => 'Nội dung'),
        array('data' => 'Thao tác'),
	);

	$nodes = node_load_multiple(array(),array('type' => 'slides'));
	$index = 1;
	$rows = array();

	foreach($nodes as $node) {
		// Link file
		if(isset($node->field_slide_file[$node->language][0]['filename']) && !empty($node->field_slide_file[$node->language][0]['filename'])) {
			$file_slide = $node->field_slide_file[$node->language][0]['filename'];
		} else {
			$file_slide = '';
		}
		// The loai video
		if(isset($node->field_media_type[$node->language][0]['value']) && !empty($node->field_media_type[$node->language][0]['value'])) {
			$media_type = $node->field_media_type[$node->language][0]['value'];
			if($media_type == 1) {
				$media_type = 'Video';
			}
			if($media_type == 2) {
				$media_type = 'Live Streaming';
			}
			if($media_type == 3) {
				$media_type = 'Image';
			}
		} else {
			$media_type = '';
		}
		// Thoi luong video
		if(isset($node->field_file_time[$node->language][0]['value']) && !empty($node->field_file_time[$node->language][0]['value'])) {
			$file_time = $node->field_file_time[$node->language][0]['value'];
		} else {
			$file_time = '';
		}
        $media_path = file_create_url($node->field_slide_file['und'][0]['uri']);

        switch ($node->field_media_type['und'][0]['value']){
            case 1:
                $media = '<video width="200" controls="controls">
                                        <source src="'.$media_path.'" type="video/mp4" autostart="false">
                                  </video>';
                break;
            case 2:
                break;
            case 3:
                $media = '<img src="'.$media_path.'" width="200"/>';
                break;

        }
		$rows[]  = array(
			$index,
			'<h4><a href="/upload-noidung/'.$node->nid.'">'.$node->title.'</a></h4>',
			'<p>'.$media.'</p>',
			'<ul class="tools">
                <li><a href="'.$base_url.'/upload-noidung/edit/'.$node->nid.'" class="btn btn-warning"><i class="fa fa-edit"></i> Sửa</a></li>
             </ul>'
		); 

		$index++;
	}

	$form = array();
    $form['noidung_danhsach'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['noidung_danhsach']['header'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/upload-noidung/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Tạo nội dung</a>',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['noidung_danhsach']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['noidung_danhsach']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}
/*
* Build form to upload
*/
function upload_content_media_server($form,&$form_state,$value) {
    // Check page to right directory
    if($value !== 'add' && $value !== 'edit') {
        drupal_not_found();
    }
    drupal_add_css(drupal_get_path('module','upload_content').'/assets/css/upload.css');
    // lay id check xem co phai la node cua noi dung khong
    if($value == 'edit') {
        $path = current_path();
        $nid = arg(2,$path);
        $node = node_load($nid);
        if($node->type !== 'slides') {
            unset($node);
        }
    }

    $form = array();
    $video_id  = adtrue_session('video_variable_name');
    $form['hidden_myfile'] = array(
        '#type' => 'hidden',
        '#title' => 'tmp file',
        '#default_value' => isset($nid) ? $nid : '',
    );
    $form['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên Nội dụng'),
        '#default_value' => isset($node) ? $node->title : '',
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,

    );
    $form['media_type'] = array(
        '#type' => 'select',
        '#title' => t('Media Type'),
        '#options' => array(
            1 => t('Video'),
            2 => t('Live Streaming'),
            3 => t('Image'),
        ),
        '#default_value' => 1,
    );
    $form['file_duration'] = array(
        '#type' => 'textfield',
        '#title' => 'Thời lượng nội dung',
        '#default_value' => isset($node) ? $node->field_slide_duration['und'][0]['value'] : '',
    );

    $form['push_myfile'] =  array(
        '#title' => t('Upload video'),
        '#type' => 'managed_file',
        '#description' => t('File sẽ được đẩy lên server làm nội dung cho playlist.'),
        '#default_value' => isset($video_id) ? $video_id : NULL,
        '#upload_location' => 'public://video_upload_content/',
        '#upload_validators' => array(
            'file_validate_extensions' => array('png mp4 gif jpg txt'),
        ),
    );

    $form['video_submit'] = array(
        '#type' => 'submit',
        '#value' => 'Tải lên',
    );
    adtrue_session('video_variable_name','','delete');
    return $form;
}


/*
* Submit function
*/
function upload_content_media_server_submit($form,&$form_state) {
    drupal_add_css(drupal_get_path('module','upload_content').'/assets/css/upload.css');


    $file_info = $form_state['values']['push_myfile'];
    $nid = $form_state['values']['hidden_myfile'];
    $title = $form_state['values']['title'];
    $media_type = $form_state['values']['media_type'];
    $file_duration = $form_state['values']['file_duration'];

    if(isset($file_info)) {
        // Xoa file khi click remove button
        if($file_info === 0) {
            //Lay file hien tai de xoa
            $video_exist_id = adtrue_session('video_variable_name');
            $video_exist_file = file_load($video_exist_id);

            if(!empty($video_exist_file)) {
                file_usage_delete($video_exist_file,'upload_content','TYPE',$video_exist_id);
                // The file_delete() function takes a file object and checks to see if
                // the file is being used by any other modules. If it is the delete
                // operation is canceled, otherwise the file is deleted.
                file_delete($video_exist_file, TRUE);
                drupal_set_message(t('Đã xóa file.'));
            }
        } else {
            // Them file
            $current_video = file_load($form_state['values']['push_myfile']);

            if(!empty($current_video->fid)) {

                // Lay dia chi id hien tai cua file upload
                $file_id = $current_video->fid;
                // Dat id cho bien 'video_variable_name'
                adtrue_session('video_variable_name',$file_id);
                // $current_video->status = FILE_STATUS_PERMANENT;

                // Lay thong tin file
                $filename = $current_video->filename;
                $destination_file = time().basename($filename);
                $file_size =  $current_video->filesize;

                // Connect to host
                $conn = ftp_connect('124.158.4.201',21,90);

                if(ftp_login($conn,'data','yjfwCezPwiIb95unAV')) {
                    ftp_pasv($conn,true);

                    // Update file info to datebase
                    $uri = 'public://video_upload_content/'.$destination_file;
                    $current_video->uri = $uri;
                    $current_video->filename = $destination_file;
                    $current_video->status = FILE_STATUS_PERMANENT;
                    $save_filed = file_save($current_video);

                    // Di den file to upload
                    ftp_chdir($conn,'public_html');

                    //Update lai dung luong file
                    db_update('file_managed')
                        ->fields(array('filesize'=>$file_size))
                        ->condition('fid',$file_id)
                        ->execute();

                    //Phan tich URL
                    $realPath = file_create_url($uri);
                    $lenDF = strlen($destination_file);
                    $lenDF = (int) $lenDF * -1;
                    $dirDF = substr($realPath, 0,$lenDF);
                    $dirDF = str_replace($GLOBALS['base_root'].'/', '', $dirDF);

                    // Bat dau luu file toi server khac
                    $path = ftp_pwd($conn);

                    $pss = ftp_put($conn,$path.'/'.$destination_file,$dirDF.$filename,FTP_BINARY);
                    if(!$pss) {
                        $pss = ftp_put($conn,$path.'/'.$destination_file,$dirDF.$filename,FTP_ASCII);
                    }
                    // Neu upload 2 lan failed, thong bao upload lai
                    if(!$pss) {
                        form_set_error(NULL,'Chưa thể upload file, vui lòng thử lại lần nữa');
                    }

                    // Xoa file sau khi upload
                    if(isset($nid) && !empty($nid)) {
                        $node = node_load($nid);
                        $node->title = $title;
                        $node->field_media_type[$node->language][0]['value'] = $media_type;
                        $node->field_slide_duration[$node->language][0]['value'] = $file_duration;
                        $node->field_slide_file[$node->language][0]['fid'] = $file_id;
                        $node->field_slide_file[$node->language][0]['display'] = 1;
                        node_save($node);
                    } else {
                        $node = new stdClass;
                        $node->type = 'slides';
                        $node->title = $title;
                        $node->field_media_type[$node->language][0]['value'] = $media_type;
                        $node->field_slide_duration[$node->language][0]['value'] = $file_duration;
                        $node->language = LANGUAGE_NONE;
                        $node->field_slide_file[$node->language][0]['fid'] = $file_id;
                        $node->field_slide_file[$node->language][0]['display'] = 1;
                        node_save($node);
                    }
                    // Kiem tra tat ca thang cong chua
                    if($pss) {
                        drupal_set_message('Tải nội dung lên thành công');
                    }
                }
                ftp_close($conn);

            }
        }
    }
}

function adtrue_session($key,$value = NULL,$action = '') {
    static $adtrue_storage;

    if($value) {
        $adtrue_storage[$key] = $value;
        $_SESSION['adtrue_variable'][$key] = $value;
    } elseif(empty($adtrue_storage[$key]) && isset($_SESSION['adtrue_variable'][$key])) {
        $adtrue_storage[$key] = $_SESSION['adtrue_variable'][$key];
    }
    if($action == 'delete') {
        unset($adtrue_storage[$key]);
        unset($_SESSION['adtrue_variable'][$key]);
    }
    return $adtrue_storage[$key];
}