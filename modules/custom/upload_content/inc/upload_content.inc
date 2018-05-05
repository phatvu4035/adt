<?php
/*
* Build form to upload
*/
function upload_content_media_server($form,&$form_state,$value) {

    drupal_add_css(drupal_get_path('module','upload_content').'/assets/css/upload.css');
    
    // Trường hợp file sau khi submit không thể detech được thời lượng thì sẽ truyền vào biến rebuild vào form để người dùng nhập trực tiếp
    if(isset($form_state['storage']['result']['file_duration']) && $form_state['storage']['result']['file_duration'] == 0) {
        $is_rebuild = 1;
    } else {
        $is_rebuild = 0;
    }
    // lay id check xem co phai la node cua noi dung khong
    $node = node_load($value);
    if($node) {
        if($node->type !== 'slides') {
            unset($node);
        }
    }
    return build_form_upload_content($node,$is_rebuild);
    
}

function add_content_media_server($form,&$form_state) {
    if(isset($form_state['storage']['result']['file_duration']) && $form_state['storage']['result']['file_duration'] == 0) {
        $is_rebuild = 1;
    } else {
        $is_rebuild = 0;
    }
    return build_form_upload_content('',$is_rebuild);
}

function build_form_upload_content($node = '',$is_rebuild = 0) {
    $form = array();
    $media = '';
    $media_filename = '';

    $config = config_upload_content();
    if($node) {

        $video_id = isset($node->field_slide_file['und'][0]['fid']) ? $node->field_slide_file['und'][0]['fid'] : 0;
        $nid = $node->nid;

        if(isset($node->field_slide_duration['und'][0]['value']) && !empty($node->field_slide_duration['und'][0]['value'])) {
            $duration = $node->field_slide_duration['und'][0]['value'];
        } else {
            $duration = '';
        }
        $cat_id = isset($node->field_slide_category_content['und'][0]['tid']) ? $node->field_slide_category_content['und'][0]['tid'] : '';
        $media_filename = (isset($node->field_slide_file['und'][0]['filename'])) ? $node->field_slide_file['und'][0]['filename'] : '';

        $link_open_file = (isset($node->field_link_open_file['und'][0]['value'])) ? $node->field_link_open_file['und'][0]['value'] : file_create_url($node->field_slide_file['und'][0]['uri']);

        if(isset($media_type)) {
            switch ((int)$media_type){
                case 1:
                    $media = '<video width="500" controls="controls">
                                    <source src="'.$link_open_file.'" type="video/mp4" autostart="false">
                              </video>';
                    $preview = TRUE;
                    break;
                case 2:

                    break;
                case 3:
                    $media = '<img src="'.$link_open_file.'" width="500"/>';
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

    $form['upload_info']['body']['col_right']['category_type'] = array(
        '#type' => 'select',
        '#title' => t('Danh mục'),
        '#options' => $category_type,
        '#default_value' => isset($cat_id) ? $cat_id : 0,
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

    $form['upload_info']['body']['col_right']['push_myfile'] =  array(
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
        '#required' => TRUE,
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

    $form['#submit'][] = 'upload_content_media_server_submit';
    $form['#validate'][] = 'upload_content_media_server_validate';
    return $form;
}

/* Thay doi title cua field thời lượng */
function change_title_file_duration($form,$form_state) {
    $media_type = $form_state['values']['media_type'];
    if((int)$media_type == 3) {
        $form['upload_info']['body']['col_right']['info']['file_duration']['#title'] = 'Thời gian trình chiếu ảnh';
        $form['upload_info']['body']['col_right']['info']['file_duration']['#prefix'] = '<div class="col-md-6" id="file_duration">';
    }
    return $form['upload_info']['body']['col_right']['info']['file_duration'];
}
// Form xac nhan xoa noi dung
function delete_content_media_server($form,&$form_state,$value) {
    global $base_url;
    $node = node_load($value);
    if($node->type !== 'slides') {
        drupal_not_found('Không tìm thấy trang');
    }
    if($node){
        $form['content_media'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div>',
            '#suffix' => '</div>'

        );
        $form['content_media']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-10">Bạn có chắc chắn muốn xóa nội dung ? </h3> <a href="'.$base_url.'/danhmuc/noidung/list" class="btn btn-info pull-right" style="margin-top:24px;"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['content_media']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );
        $form['content_media']['body']['name_upload_content'] = array(
            '#type' => 'item',
            '#markup' => '<b>Tên nội dung : </b>' .$node->title,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );

        $link_open_file = (isset($node->field_link_open_file['und'][0]['value'])) ? $node->field_link_open_file['und'][0]['value'] : file_create_url($node->field_slide_file['und'][0]['uri']);
        $media = '';
        if(isset($node->field_media_type['und'][0]['value'])) {
            switch ((int)$node->field_media_type['und'][0]['value']){
                case 1:
                    $media = '<video width="200" controls="controls">
                                            <source src="'.$link_open_file.'" type="video/mp4" autostart="false">
                                      </video>';
                    break;
                case 2:

                    break;
                case 3:
                    $media = '<img src="'.$link_open_file.'" width="200"/>';
                    break;

            }
        }

        $form['content_media']['body']['slide_media'] = array(
            '#type' => 'item',
            '#markup' => $media,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['content_media']['body']['id_upload_content'] = array(
            '#type' => 'hidden',
            '#value' => $node->nid,
        );
        
        $form['content_media']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );

        $form['content_media']['footer']['delete_slide'] = array(
            '#type' => 'submit',
            '#value' => 'Xóa nội dung',
            '#attributes' => array(
                'class' => array('btn','btn-danger')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>'
        );

        return $form;
    }
}

function delete_content_media_server_submit($form,&$form_state) {
    $noidung_nid = $form_state['values']['id_upload_content'];
    $query = db_select('field_data_field_slides_playlist','slide_playlist');
    $query->condition('field_slides_playlist_nid',$noidung_nid);
    $query->fields('slide_playlist',array('entity_id'));
    $result = $query->execute();
    // Kiem tra xem noi dung co o trong 1 playlist nao khong
    if($result->rowCount()) {
        drupal_set_message('Nội dung đã ở trong playlist, không thể xóa', 'error');
        $form_state['redirect'] = 'noidung/list';
        return;
    }

    $node = node_load($noidung_nid);
    if($node->type == 'slides')  {
        node_delete($noidung_nid);
        drupal_set_message('Đã xóa nội dung');
    } else {
        drupal_set_message('Không tìm thấy nội dung','error');
    }
}

function convert_vi_to_en($str) {
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

function remove_special_character($string) {
    $string = str_replace(' ', '-', $string); // Replaces all spaces with hyphens.
    return preg_replace('/[^A-Za-z1-9]/', '', $string); // Removes special chars.
}


function upload_content_file_insert($file) {
    if($file->source == 'push_myfile') {
        $uri = $file->uri;
        $uri = convert_vi_to_en($uri);
        $basename = drupal_basename($uri);

        $filename_length = -1 * strlen($basename);
        $upload_loc = substr($uri,0,$filename_length);
        $current_time = time();
        // lấy tên file
        $ext = pathinfo($basename, PATHINFO_EXTENSION);
        $filename = $current_time.rand(1,2);
        $filename = remove_special_character($filename).'.'.$ext;
        $upload_loc .= $filename;
        $file = file_move($file,$upload_loc);
    }
}

function upload_content_media_server_validate($form,&$form_state) {
    $file_info = $form_state['values']['push_myfile'];

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
               form_set_error('file_duration','Định dạng thời gian không đúng ');
            }

        } elseif(count($duration_in_array) > 3) {
            form_set_error('file_duration','Định dạng thời gian không đúng');
        } else {
            // Định dạng thời gian tính bằng giây
            if(!is_numeric($file_duration)) {
                form_set_error('file_duration','Định dạng thời gian không đúng');
            }
        }
    }
}

/*
* Submit function
*/
function upload_content_media_server_submit($form,&$form_state) {
    drupal_add_css(drupal_get_path('module','upload_content').'/assets/css/upload.css');
    global $user;
    $file_info = $form_state['values']['push_myfile'];
    $nid = $form_state['values']['hidden_myfile'];
    $title = $form_state['values']['title'];
    $file_duration = $form_state['values']['file_duration'];

    // Xu li trường thời gian khi nhập thòi gian
    if($form_state['values']['file_duration'] !== '') {
        $file_duration = $form_state['values']['file_duration'];
        $duration_in_array = explode(':', $file_duration);
        if($duration_in_array && count($duration_in_array) > 1) {
            $str_time = $file_duration;
            sscanf($str_time, "%d:%d:%d", $hours, $minutes, $seconds);
            $file_duration = isset($seconds) ? $hours * 3600 + $minutes * 60 + $seconds : $hours * 60 + $minutes;
        }
    }
    $category_type = $form_state['values']['category_type'];

    $image_type = array('GIF','PNG','JPG','JPEG','TIFF');
    $video_type = array('MP3','MP4','PPT');
    if(isset($file_info) && $file_info) {
        //Xoa file khi click remove button
        if($file_info === 0) {
            $video_exist_file = file_load($video_exist_id);
            if(!empty($video_exist_file)) {
                file_usage_delete($video_exist_file,'upload_content','media',$video_exist_id);
                // Xoa file tren server
                // $delete_ok = moving_file_content_uploaded($video_exist_file->uri,$video_exist_file->filename,1);
                file_delete($video_exist_file, TRUE);
            }
        } else {
            // Them file
            $current_video = file_load($form_state['values']['push_myfile']);
            if(!empty($current_video->fid)) {
                // Lay dia chi id hien tai cua file upload
                $file_id = $current_video->fid;

                // Lay thong tin file
                $filename = $current_video->filename;
                $file_size =  $current_video->filesize;
                $file_volume = $file_size;

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
                    if($file_duration == '') {
                        $form_state['storage']['result']['file_duration'] = 0;
                        drupal_set_message('Có lỗi xảy ra trong quá trình upload, không thể lấy được thời lượng file. Vui lòng nhập trực tiếp','error');
                        $form_state['storage']['result']['file_duration'] = 0;
                        $form_state['rebuild'] = TRUE;
                        return;
                    }
                }

                // Kiem tra xem da upload duoc file len server chua
                // $uploaded = moving_file_content_uploaded($current_video->uri,$current_video->filename);
                // if(!$uploaded) {
                //     drupal_set_message('Không thể upload file đến remote server','error');
                //     // Neu chua upload duoc xoa file
                //     file_delete($current_video, TRUE);
                //     $form_state['rebuild'] = TRUE;
                //     return;
                // }

                //Update lai node
                if(isset($nid) && !empty($nid)) {
                    $node = node_load($nid);
                    $node->title = $title;
                    $node->field_media_type['und'][0]['value'] = $media_type;
                    $node->field_slide_duration['und'][0]['value'] = $file_duration;
                    $node->field_file_size['und'][0]['value'] = $file_volume;
                    $node->field_slide_file['und'][0]['fid'] = $file_id;
                    $node->field_slide_file['und'][0]['display'] = 1;
                    // $node->field_link_open_file['und'][0]['value'] = $uploaded;
                    $node->field_link_open_file['und'][0]['value'] = file_create_url($current_video->uri);

                    $node->field_slide_category_content['und'][0]['tid'] = $category_type;
                    node_save($node);
                    if($node->nid) {
                        $che = file_unmanaged_delete_recursive($current_video->uri);
                        drupal_set_message('Tải nội dung lên thành công');
                    }
                    $form_state['redirect'] = 'noidung/list';
                } else {
                    // tao moi 1 node
                    $node = new stdClass;
                    $node->type = 'slides';
                    $node->title = $title;
                    $node->uid = $user->uid;
                    $node->language = LANGUAGE_NONE;
                    $node->field_media_type['und'][0]['value'] = $media_type;
                    $node->field_slide_duration['und'][0]['value'] = $file_duration;
                    $node->field_file_size['und'][0]['value'] = $file_volume;
                    // $node->field_link_open_file['und'][0]['value'] = $uploaded;
                    $node->field_link_open_file['und'][0]['value'] = file_create_url($current_video->uri);

                    $node->field_slide_file['und'][0]['fid'] = $file_id;
                    $node->field_slide_file['und'][0]['display'] = 1;
                    $node->field_slide_category_content['und'][0]['tid'] = $category_type;
                    node_save($node);
                    if($node->nid) {
                        // $che = file_unmanaged_delete_recursive($current_video->uri);
                        drupal_set_message('Tải nội dung lên thành công');
                    }
                    $form_state['redirect'] = 'noidung/list';
                }

            }
        }  
    }
}

// Upload file to server
function moving_file_content_uploaded($uri,$filename,$is_delete = 0) {
    $host_config = config_upload_content();
    $ftp_host = $host_config['ftp_host'];
    $user_host = $host_config['user'];
    $pass_host = $host_config['password'];
    $port_host = $host_config['port'];
    $path_to_save = $host_config['path_to_save'];
    $url = drupal_realpath($uri);
   
    set_include_path(drupal_get_path('module','upload_content').'/lib');
    include('Net/SFTP.php');
    define('NET_SSH2_LOGGING', NET_SSH2_LOG_COMPLEX);

    $sftp = new Net_SFTP($ftp_host,$port_host);
    if(!$sftp->login($user_host,$pass_host)) {
        return false;
    } else {
        $current_dir = $sftp->pwd();
        if($is_delete) {
            // Xoa file tren server
            $output = $sftp->delete($path_to_save.$filename);
            return $output;
        } else {
            // Them file to server
            $output = $sftp->put($path_to_save.$filename,$url,NET_SFTP_LOCAL_FILE);
            $link_to_open_file = $host_config['domain_open'].$filename;
            return $link_to_open_file;
        }
    }
}



function danhsach_noidung_video($form,&$form_state) {
    global $user,$base_url;
    $voca = taxonomy_vocabulary_machine_name_load('noidung');
    $vid = $voca->vid; 
    $terms = taxonomy_term_load_multiple(array(), array('vid' => $vid));
    $ndSelect =  array(0=>t('Chọn'));
    foreach($terms as $noidung) {
      $ndSelect[$noidung->tid] = $noidung->name;
    }

    $form = array();
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Tên nội dung'),
        array('data' => 'Nội dung'),
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
        array('data' => 'Thời lượng'),
        array('data' => 'Danh mục'),
        array('data' => 'Loại nội dung'),
        array('data' => 'Thao tác'),
    );
    $rows = form_table_noidung();

    if(isset($_GET['media_type']) && (int)$_GET['media_type'] > 0) {
        $media_type = $_GET['media_type'];
    }
    if(isset($_GET['category_type']) && (int)$_GET['category_type'] > 0) {
        $category_type = $_GET['category_type'];
    }
    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
        $keyword = $_GET['keyword'];
        $category_type = 0;
        $media_type = 0;
    }

    $form = array();
    $adtrue_block = module_invoke('adtrue_block', 'block_view', 'content_dashboard');
    $adtrue_block_content = $adtrue_block['content'];
    $form = $adtrue_block_content;
    $form['noidung_danhsach'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['noidung_danhsach']['header'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['noidung_danhsach']['header']['media_type'] = array(
        '#type' => 'select',
        '#title' => t('Thể loại'),
        '#options' => array(0 => 'Chọn', 1 => 'Video', 2=> 'Live Streaming', 3 => 'Image'),
        '#default_value' => (isset($media_type)) ? $media_type : 0,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['noidung_danhsach']['header']['category_type'] = array(
        '#type' => 'select',
        '#title' => t('Loại nội dung'),
        '#options' => $ndSelect,
        '#default_value' => (isset($category_type)) ? $category_type : 0,
         '#prefix' => '<div class="col-md-2 box-search-item">',
         '#suffix' => '</div>',
    );

    $form['noidung_danhsach']['header']['keyword'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên nội dung'),
        '#options' => $ndSelect,
        '#default_value' => (isset($keyword)) ? $keyword : null,
        '#prefix' => '<div class="col-md-3 box-search-item">',
         '#suffix' => '</div>',
    );
    $form['noidung_danhsach']['header']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Tìm kiếm'),
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['noidung_danhsach']['header']['create_upload'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/noidung/add" class="btn btn-success"><i class="fa fa-plus"></i> Tạo nội dung</a>',
        '#prefix' => '<div class="col-md-2 box-search-item">',
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
    $form['#method'] = 'GET';
    return $form;
}

function form_table_noidung() {
    $limit = 20;
    global $base_url;
    global $user;
    $query = db_select('node','n')->extend('PagerDefault')->distinct()->limit($limit);
    $query->condition('n.type','slides');
    $notStr = TRUE;

    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
       $keyword = $_GET['keyword'];
       $keyword = trim($keyword);
       $query->condition('n.title','%'.db_like($keyword).'%','LIKE');
       $notStr = FALSE;
    }

    if(isset($_GET['media_type']) && !empty($_GET['media_type']) && $notStr){
        $media_type = $_GET['media_type'];
        $query->leftJoin('field_data_field_media_type','mtype','mtype.entity_id = n.nid');
        $query->condition('mtype.field_media_type_value',$media_type);
    }

    if(isset($_GET['category_type']) && !empty($_GET['category_type']) && $notStr){
        $cat_type = $_GET['category_type'];
        $query->leftJoin('field_data_field_slide_category_content','ctype','ctype.entity_id = n.nid');
        $query->condition('ctype.field_slide_category_content_tid',$cat_type);
    }
    if(isset($_GET['page']) && (int)$_GET['page'] > 0) {
        $page  = (int)$_GET['page'];
        $index = $limit*$page + 1;
    } else {
        $index = 1;
    }

    $query->fields('n',array('nid'));
    $query->orderBy('n.changed','DESC') ;
    $result = $query->execute();
    if($result->rowCount()) {
        foreach($result as $row) {
            $node = node_load($row->nid);
            $uid = $node->uid;

            // Link file
            if(isset($node->field_slide_file['und'][0]['filename']) && !empty($node->field_slide_file['und'][0]['filename'])) {
                $file_slide = $node->field_slide_file['und'][0]['filename'];
            } else {
                $file_slide = '';
            }
            // The loai video
            if(isset($node->field_media_type['und'][0]['value']) && !empty($node->field_media_type['und'][0]['value'])) {
                $media_type = $node->field_media_type['und'][0]['value'];
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

            $catnd_id = isset($node->field_slide_category_content['und'][0]['tid']) ? $node->field_slide_category_content['und'][0]['tid'] : 0;
            $catnd = taxonomy_term_load($catnd_id);
            // Thoi luong video
            if(isset($node->field_file_time['und'][0]['value']) && !empty($node->field_file_time['und'][0]['value'])) {
                $file_time = $node->field_file_time['und'][0]['value'];
            } else {
                $file_time = '';
            }

            $media_filename = '';
            $media = '';
            $config = config_upload_content();

            $link_open_file = (isset($node->field_link_open_file['und'][0]['value'])) ? $node->field_link_open_file['und'][0]['value'] : file_create_url($node->field_slide_file['und'][0]['uri']);

            if(isset($node->field_media_type['und'][0]['value'])) {
                switch ((int)$node->field_media_type['und'][0]['value']){
                    case 1:
                        $media = '<video width="250" controls="controls">
                                                <source src="'.$link_open_file.'" type="video/mp4" autostart="false">
                                          </video>';
                        break;
                    case 2:

                        break;
                    case 3:
                        $media = '<img src="'.$link_open_file.'" width="250"/>';
                        break;

                }
            }

            // Ngay tao
            $created = date('d/m/Y H:i:s',$node->created);
            $uid = $node->uid;
            $user_node = user_load($uid);

            if(isset($node->field_slide_duration['und'][0]['value'])) {
                $duration = $node->field_slide_duration['und'][0]['value'];
                $duration = ceil($duration);
                // Tính số phút
                $minute = $duration / 60;
                $minute = floor($minute); 
                if($minute < 60) {
                   $thoiluong =  $minute.':'.($duration - $minute * 60);
                } else if($minute > 60) {
                    $hour = $minute / 60;
                    $hour = floor($hour);
                    $remain_minute = $minute - ($hour * 60);
                    $thoiluong = $hour . ':'.$remain_minute.':'.($duration - $minute * 60);
                }

            } else {
                $thoiluong = '';
            }

            $rows[]  = array(
                $index,
                '<p style="max-width:250px;"><a href="'.$link_open_file.'">'.$node->title.'</a></p>',
                '<p>'.(isset($media) && $media) ? $media : ''.'</p>',
                $user_node->name,
                $created,
                $thoiluong,
                (isset($catnd) && $catnd) ? $catnd->name : '',
                $media_type,
                 '<div class="btn-group">
                      <button type="button" class="btn btn-success">Thao tác</button>
                      <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                          <span class="caret"></span>
                          <span class="sr-only">Toggle Dropdown</span>
                      </button>
                      <ul class="dropdown-menu" role="menu">
                          <li><a href="'.$base_url.'/noidung/edit/'.$node->nid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                          <li><a href="'.$base_url.'/noidung/delete/'.$node->nid.'" ><i class="fa fa-trash"></i> Xóa</a></li>
                      </ul>
                  </div>'
            ); 

            $index++;
        }

        return isset($rows) ? $rows : array();
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
    return isset($adtrue_storage[$key]) ? $adtrue_storage[$key] : '';
}