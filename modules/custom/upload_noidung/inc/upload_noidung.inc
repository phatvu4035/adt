<?php
function add_noidung_media_server($form,&$form_state) {
    if(isset($form_state['storage']['result']['file_duration']) && $form_state['storage']['result']['file_duration'] == 0) {
        $is_rebuild = 1;
    } else {
        $is_rebuild = 0;
    }
    return build_form_upload_noidung('',$is_rebuild);
}

function build_form_upload_noidung($node = '',$is_rebuild = 0) {

    $form = array();
    $media = '';
    $media_filename = '';
    $config = config_upload_noidung();
    if($node) {

        $video_id = isset($node->field_slide_file['und'][0]['fid']) ? $node->field_slide_file['und'][0]['fid'] : 0;
        $nid = $node->nid;

        if(isset($node->field_slide_duration['und'][0]['value']) && !empty($node->field_slide_duration['und'][0]['value'])) {
            $duration = $node->field_slide_duration['und'][0]['value'];
        } else {
            $duration = '';
        }
        $media_type = isset($node->field_media_type['und'][0]['value']) ? $node->field_media_type['und'][0]['value'] : '';
        $cat_id = isset($node->field_slide_category_content['und'][0]['tid']) ? $node->field_slide_category_content['und'][0]['tid'] : '';
        $media_filename = (isset($node->field_slide_file['und'][0]['filename'])) ? $node->field_slide_file['und'][0]['filename'] : '';
        
        if(isset($media_type)) {
            switch ((int)$media_type){
                case 1:
                    $media = '<video width="500" controls="controls">
                                            <source src="'.$config['domain_open'].$media_filename.'" type="video/mp4" autostart="false">
                                      </video>';
                    $preview = TRUE;
                    break;
                case 2:

                    break;
                case 3:
                    $media = '<img src="'.$config['domain_open'].$media_filename.'" width="500"/>';
                    $preview = TRUE;
                    break;
            }
        }
    }

    $voca = taxonomy_vocabulary_machine_name_load('noidung');
    $vid = $voca->vid;
    $categories = taxonomy_term_load_multiple(array(),array('vid' => $vid));

    $category_type = array(0 => 'Chọn');
    foreach ($categories as $cat) {
        $category_type[$cat->tid] = $cat->name;
    }

    $form['hidden_myfile'] = array(
        '#type' => 'hidden',
        '#title' => 'tmp file',
        '#default_value' => (isset($nid) && $nid) ? $nid : '',
    );
    $form['upload_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    
    $form['upload_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );

    $form['upload_info']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Chi tiết nội dung</h3>'
    );


    $form['upload_info']['body'] = array(
            '#type' => 'container',
            '#attributes' => array(
                'class' => array('box-success'),
            ),
    );
    $form['upload_info']['body']['col_left'] = array(
        '#type' => 'container',
        '#weight' => 1,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>',
    );
    $form['upload_info']['body']['col_right'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>',
    );
    $form['upload_info']['body']['col_left']['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên Nội dụng'),
        '#default_value' => (isset($node) && $node) ? $node->title : '',
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
    );
    $form['upload_info']['body']['col_left']['description'] = array(
        '#type' => 'textarea',
        '#title' => 'Mô tả',
    );

    $form['upload_info']['body']['col_left']['preview'] = array(
        '#type' => 'item',
        '#title' => (isset($preview) && $preview) ? 'Xem trước' : '',
        '#markup' => '<div id="preview_mymedia">'.$media.'</div>',
    );

    $form['upload_info']['body']['col_right']['media_type'] = array(
        '#type' => 'select',
        '#title' => t('Media Type'),
        '#ajax' => array(
          'callback' => 'change_title_file_duration_1',
          'method' =>  'replace',
          'wrapper' => 'file_duration',
        ),
        '#options' => array(1 => 'Video', 2=> 'Live Streaming', 3 => 'Image'),
        '#default_value' => isset($media_type) ? $media_type : 1,
    );

    $form['upload_info']['body']['col_right']['category_type'] = array(
        '#type' => 'select',
        '#title' => t('Danh mục'),
        '#options' => $category_type,
        '#default_value' => isset($cat_id) ? $cat_id : 0,
    );
   
    $form['upload_info']['body']['col_right']['media_link'] = array(
        '#type' => 'textfield',
        '#title' => 'Streaming link',
        '#states' => array(
            'visible' => array( // action to take.
                ':input[name="media_type"]' => array('value' => 2),
            ),
        ),
    );

    $form['upload_info']['body']['col_right']['info'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-info','row')
        ),
    );
    if($is_rebuild == 1) {
        $form['upload_info']['body']['col_right']['info']['file_duration'] = array(
            '#type' => 'textfield',
            '#title' => 'Thời gian',
            '#default_value' => isset($duration) ? $duration : '',
            '#prefix' => '<div class="col-md-6" id="file_duration">',
            '#suffix' => '</div>'
        );
    } else {
        $form['upload_info']['body']['col_right']['info']['file_duration'] = array(
            '#type' => 'textfield',
            '#title' => 'Thời gian',
            '#default_value' => isset($duration) ? $duration : '',
            '#attributes' => array(
                'placeholder' => 'Định dạng h:m:s hoặc số giây',
            ),
            '#prefix' => '<div class="col-md-6 file_duration_hidden" id="file_duration">',
            '#suffix' => '</div>'
        );
    }

    $form['upload_info']['body']['col_right']['push_myfile_2'] =  array(
        '#title' => t('Upload file'),
        '#type' => 'managed_file',
        '#description' => t('File sẽ được đẩy lên server làm nội dung cho playlist.'),
        '#default_value' => isset($video_id) ? $video_id : NULL,
        '#upload_location' => 'public://video_upload_content/',
        '#upload_validators' => array(
            'file_validate_extensions' => array('png mp4 gif jpg txt mp3 ppt'),
        ),
        '#states' => array(
            'invisible' => array( // action to take.
                ':input[name="media_type"]' => array('value' => 2),
            ),
        ),
    );
    $form['upload_info']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer with-border">',
            '#suffix' => '</div>'
    );
    $form['upload_info']['footer']['video_submit'] = array(
        '#type' => 'submit',
        '#value' => 'Lưu lại',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-success')
        ),
    );

    $form['#submit'][] = 'upload_noidung_media_server_submit';
    $form['#validate'][] = 'upload_noidung_media_server_validate';
    return $form;
}

/* Thay doi title cua field thời lượng */
function change_title_file_duration_1($form,$form_state) {
    $media_type = $form_state['values']['media_type'];
    if((int)$media_type == 3) {
        $form['upload_info']['body']['col_right']['info']['file_duration']['#title'] = 'Thời gian trình chiếu ảnh';
        $form['upload_info']['body']['col_right']['info']['file_duration']['#prefix'] = '<div class="col-md-6" id="file_duration">';
    }
    return $form['upload_info']['body']['col_right']['info']['file_duration'];
}
// Form xac nhan xoa noi dung
function delete_noidung_media_server($form,&$form_state,$value) {
    $form = array();
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => '<h4>Vui lòng xác nhận để hủy nội dung</h4>',
    );
    $form['nd_nid'] = array(
        '#type' => 'hidden',
        '#value' => $value,
    );
    return confirm_form($form,'Vui lòng xác nhận để xóa nội dung','tainoidung/list',t(''),t('Xác nhận'),t('Hủy bỏ'));
}


function convert_vi_to_en_1($str) {
    $str = preg_replace("/(à|á|ạ|ả|ã|â|ầ|ấ|ậ|ẩ|ẫ|ă|ằ|ắ|ặ|ẳ|ẵ)/", "a", $str);
    $str = preg_replace("/(è|é|ẹ|ẻ|ẽ|ê|ề|ế|ệ|ể|ễ)/", 'e', $str);
    $str = preg_replace('/(ì|í|ị|ỉ|ĩ)/', 'i', $str);
    $str = preg_replace('/(ò|ó|ọ|ỏ|õ|ô|ồ|ố|ộ|ổ|ỗ|ơ|ờ|ớ|ợ|ở|ỡ)/', 'o', $str);
    $str = preg_replace('/(ù|ú|ụ|ủ|ũ|ư|ừ|ứ|ự|ử|ữ)/','u', $str);
    $str = preg_replace('/(ỳ|ý|ỵ|ỷ|ỹ)/','y', $str);
    $str = preg_replace('/(đ)/','d', $str);
    $str = preg_replace('/(À|Á|Ạ|Ả|Ã|Â|Ầ|Ấ|Ậ|Ẩ|Ẫ|Ă|Ằ|Ắ|Ặ|Ẳ|Ẵ)/', 'A', $str);
    $str = preg_replace('/(È|É|Ẹ|Ẻ|Ẽ|Ê|Ề|Ế|Ệ|Ể|Ễ)/', 'E', $str);
    $str = preg_replace('/(Ì|Í|Ị|Ỉ|Ĩ)/','I', $str);
    $str = preg_replace('/(Ò|Ó|Ọ|Ỏ|Õ|Ô|Ồ|Ố|Ộ|Ổ|Ỗ|Ơ|Ờ|Ớ|Ợ|Ở|Ỡ)/', 'O', $str);
    $str = preg_replace('/(Ù|Ú|Ụ|Ủ|Ũ|Ư|Ừ|Ứ|Ự|Ử|Ữ)/','U', $str);
    $str = preg_replace('/(Ỳ|Ý|Ỵ|Ỷ|Ỹ)/','Y', $str);
    $str = preg_replace('/(Đ)/', 'D', $str);
    //$str = str_replace(” “, “-”, str_replace(“&*#39;”,”",$str));
    return $str;
}

function remove_special_character_1($string) {
    $string = str_replace(' ', '-', $string); // Replaces all spaces with hyphens.
    return preg_replace('/[^A-Za-z1-9]/', '', $string); // Removes special chars.
}


function upload_noidung_file_insert($file) {
    if($file->source == 'push_myfile_2') {
        $uri = $file->uri;
        $uri = convert_vi_to_en_1($uri);
        $basename = drupal_basename($uri);

        $filename_length = -1 * strlen($basename);
        $upload_loc = substr($uri,0,$filename_length);
        $current_time = time();
        // lấy tên file
        $ext = pathinfo($basename, PATHINFO_EXTENSION);
        $filename = $current_time.rand(1,2);
        $filename = remove_special_character_1($filename).'.'.$ext;
        $upload_loc .= $filename;
        debug($upload_loc);
        debug($file);
        $file = file_move($file,$upload_loc);
    }
}

function upload_noidung_media_server_validate($form,&$form_state) {
    $file_info = $form_state['values']['push_myfile_2'];

    if($form_state['values']['media_link'] == '' && $form_state['values']['media_type'] !== 2 && !$file_info) {
        form_set_error('push_myfile_2','Chưa có file được upload');
    }
    if($form_state['values']['media_type'] == 2 && $form_state['values']['media_link'] == '') {
        form_set_error('media_link','Media link chưa được nhập');
    }
    if($form_state['values']['file_duration'] !== '') {
        $file_duration = $form_state['values']['file_duration'];
        $duration_in_array = explode(':', $file_duration);
        $number = TRUE;

        if($duration_in_array && count($duration_in_array) > 1) {
            // Định dạng thời gian có giờ phút ngày
            foreach($duration_in_array as $thoigian) {
                if(!is_numeric($thoigian)|| (int)$thoigian > 60) {
                   $number = FALSE;
                }
            }
            if(!$number) {
               form_set_error('file_duration','Định dạng thời gian không đúng c');
            }

        } elseif(count($duration_in_array) > 3) {
            form_set_error('file_duration','Định dạng thời gian không đúng b');
        } else {
            // Định dạng thời gian tính bằng giây
            if(!is_numeric($file_duration)) {
                form_set_error('file_duration','Định dạng thời gian không đúng a');
            }
        }
    }
}
/*
* Submit function
*/
function upload_noidung_media_server_submit($form,&$form_state) {
    global $user;
    $file_info = $form_state['values']['push_myfile_2'];
    $nid = $form_state['values']['hidden_myfile'];
    $title = $form_state['values']['title'];
    $file_duration = $form_state['values']['file_duration'];

    $media_type = $form_state['values']['media_type'];
    $category_type = $form_state['values']['category_type'];
    $media_link = $form_state['values']['media_link'];

    $image_type = array('GIF','PNG','JPG','JPEG','TIFF');
    $video_type = array('MP3','MP4','PPT');

    if(isset($file_info) && $file_info && (int)$media_type !== 2) {
        //Xoa file khi click remove button
        if($file_info === 0) {
            $video_exist_file = file_load($video_exist_id);
            if(!empty($video_exist_file)) {
                file_usage_delete($video_exist_file,'upload_noidung','media',$video_exist_id);
                // Xoa file tren server
                $delete_ok = moving_file_content_uploaded_1($video_exist_file->uri,$video_exist_file->filename,1);
                file_delete($video_exist_file, TRUE);
            }
        } else {
            // Them file
            $current_video = file_load($form_state['values']['push_myfile_2']);
            if(!empty($current_video->fid)) {
                // Lay dia chi id hien tai cua file upload
                $file_id = $current_video->fid;

                // Lay thong tin file
                $filename = $current_video->filename;
                $file_size =  $current_video->filesize;
                $file_volume = $file_size;

                // Update file info to database
                file_usage_add($current_video, 'upload_noidung', 'media', $file_id);

                // Neu file da ton tai san va co status = 1, khi nguoi dung khong sua file thi cho pass qua
                $ignore_save  = FALSE;
                if($current_video->status) {
                    $ignore_save = TRUE;
                }
                if($ignore_save) {
                    $save_filed = $current_video;
                } else {
                    $current_video->status = FILE_STATUS_PERMANENT;
                    $save_filed = file_save($current_video);
                }

                // Thay đổi tên file
                $uri = $save_filed->uri;
                $ext = pathinfo($save_filed->uri,PATHINFO_EXTENSION);
                if(in_array(strtoupper($ext),$image_type)) {

                    $media_type = 3;
                    // Nếu không có trường duration mặc định là 60s
                    if($file_duration == '') {
                        $file_duration = 60;
                    }
                } elseif (in_array(strtoupper($ext),$video_type)) {

                    $media_type = 1;
                }

                // Kiem tra xem da upload duoc file len server chua
                $is_uploaded = moving_file_content_uploaded_1($current_video->uri,$current_video->filename);
                if(!$is_uploaded) {
                    drupal_set_message('Không thể upload file đến remote server','error');
                    // Neu chua upload duoc xoa file
                    file_delete($current_video, TRUE);
                    $form_state['rebuild'] = TRUE;
                }

            }
        }  
    }

}

// Upload file to server
function moving_file_content_uploaded_1($uri,$filename,$is_delete = 0) {
    $host_config = config_upload_noidung();
    $ftp_host = $host_config['ftp_host'];
    $user_host = $host_config['user'];
    $pass_host = $host_config['password'];
    $port_host = $host_config['port'];
    $path_to_save = $host_config['path_to_save'];
    $url = drupal_realpath($uri);
   
    set_include_path(drupal_get_path('module','upload_noidung').'/lib');
    include('Net/SFTP.php');
    define('NET_SSH2_LOGGING', NET_SSH2_LOG_COMPLEX);

    $sftp = new Net_SFTP($ftp_host,$port_host);
    if(!$sftp->login($user_host,$pass_host)) {
        drupal_set_message('false');
        return false;
    } else {
        $sftp->pwd();
        if($is_delete) {
            // Xoa file tren server
            $output = $sftp->delete($path_to_save.$filename);
            return $output;
        } else {
            // Them file to server
            $output = $sftp->put($path_to_save.$filename,$url,NET_SFTP_LOCAL_FILE);
            return $output;
        }
    }
}

// Upload file to server
// function moving_file_content_uploaded($uri,$filename,$is_delete = 0) {
//     $host_config = config_upload_noidung();
//     $ftp_host = $host_config['ftp_host'];
//     $user_host = $host_config['user'];
//     $pass_host = $host_config['password'];
//     $port_host = $host_config['port'];
//     $path_to_save = $host_config['path_to_save'];

//     $url = file_create_url($uri);
//     $conn = ftp_connect($ftp_host,$port_host,90);
//     $login_result = ftp_login($conn,$user_host,$pass_host);

//     if(!$login_result) {
//         return false;
//     }

//     ftp_pasv($conn, true);
//     ftp_chdir($conn,$path_to_save);
//     $current_path_tosave = ftp_pwd($conn);
//     $full_path_save = $current_path_tosave.'/'.$filename;

//     // Xoa file
//     if($is_delete) {
//         $delete_ok = ftp_delete($conn,$full_path_save);
//         if($delete_ok) {
//             return TRUE;
//         }
//         return FALSE;
//     }
//     // Truong hop khi vao edit ma chua sua gi va luu lai
//     $file_exist = ftp_nlist($conn,$filename);
//     if($file_exist) {
//         return 2;
//     }

//     $is_uploaded = ftp_put($conn,$full_path_save,$url,FTP_BINARY);
//     if($is_uploaded) {
//         ftp_close($conn);
//         return 1;
//     } else {
//         $is_uploaded = ftp_put($conn,$full_path_save,$url,FTP_ASCII);
//         if($is_uploaded) {
//             ftp_close($conn);
//             return 1;
//         } else {
//             return false;
//         }
//     }
// }
