<?php
// Them photo holder
function photo_holder_add($form,&$form_state) {
	return build_form_upload_photo_holder();
}

//Sua photo holder
function edit_photo_holder($form,&$form_state,$value) {
	$node = node_load($value);
    if($node) {
        if($node->type !== 'photo_holder') {
            $node = '';
        }
    }
	return build_form_upload_photo_holder($node);
}

function build_form_upload_photo_holder($node = '',$is_rebuild = 0) {
	$form = array();
	$media = '';
    $media_filename = '';

	if($node) {
		$video_id = isset($node->field_file_photo_holder['und'][0]['fid']) ? $node->field_file_photo_holder['und'][0]['fid'] : 0;
		$nid = $node->nid;
	}
	$form['hidden_photoholder'] = array(
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
        '#markup' => '<h3>Chi tiết ảnh Photo Holder</h3>'
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
    $form['upload_info']['body']['col_left']['photo_title'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên ảnh'),
        '#default_value' => (isset($node) && $node) ? $node->title : '',
        '#size' => 60,
        '#maxlength' => 128,
        '#required' => TRUE,
    );
    $form['upload_info']['body']['col_right']['photo_holder'] = array(
        '#title' => t('Photo Holder'),
        '#type' => 'managed_file',
        '#description' => t('File sẽ được đẩy lên server làm nội dung chuyển đổi giữa 2 nội dung'),
        '#default_value' => isset($video_id) ? $video_id : NULL,
        '#upload_location' => 'public://video_upload_content/',
        '#upload_validators' => array(
            'file_validate_extensions' => array('png gif jpg  ppt'),
        ),
    );
    $form['upload_info']['body']['col_right']['footer'] = array(
    	'#type' => 'submit',
        '#value' => 'Lưu lại',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-success')
        ),
    );

    $form['#submit'][] = 'photo_holder_upload_submit';
    $form['#validate'][] = 'photo_holder_upload_validate';
    return $form;
}

function adtrue_photo_holder_file_insert($file) {
    if($file->source == 'photo_holder') {
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
// Validate function
function photo_holder_upload_validate($form,&$form_state) {

}

// Submit function
function photo_holder_upload_submit($form,&$form_state) {
    global $user;
	$file_info = $form_state['values']['photo_holder'];
    $nid = $form_state['values']['hidden_photoholder'];
    $title = $form_state['values']['photo_title'];

    if(isset($file_info) && $file_info) {
    	if($file_info === 0) {
    		$video_exist_file = file_load($video_exist_id);
    		if(!empty($video_exist_file)) {
    			file_usage_delete($video_exist_file,'upload_content','media',$video_exist_id);
    			file_delete($video_exist_file, TRUE);
    		}
		} else {
			// Them file
			$current_video = file_load($file_info);
			if(!empty($current_video->fid)) {
				// Lay dia chi id hien tai cua file upload
            	$file_id = $current_video->fid;

            	// Lay thong tin file
                $filename = $current_video->filename;
                $file_size =  $current_video->filesize;

                // Update file info to database
            	file_usage_add($current_video, 'upload_content', 'media', $file_id);

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

                //Update lai node
                if(isset($nid) && !empty($nid)) {
                    $node = node_load($nid);
                    $node->title = $title;
                    $node->field_file_photo_holder['und'][0]['fid'] = $file_id;
                    $node->field_file_photo_holder['und'][0]['display'] = 1;
                    node_save($node);
                    if($node->nid) {
                        // $che = file_unmanaged_delete_recursive($current_video->uri);
                        drupal_set_message('Tải nội dung lên thành công');
                    }
                    $form_state['redirect'] = 'photo_holder/list';
                } else {
                    // tao moi 1 node
                    $node = new stdClass;
                    $node->title = $title;
                    $node->type = 'photo_holder';
                    $node->uid = $user->uid;
                    $node->language = LANGUAGE_NONE;
                    $node->title = $title;
                    $node->field_file_photo_holder['und'][0]['fid'] = $file_id;
                    $node->field_file_photo_holder['und'][0]['display'] = 1;
                    node_save($node);
                    if($node->nid) {
                        // $che = file_unmanaged_delete_recursive($current_video->uri);
                        drupal_set_message('Tải nội dung lên thành công');
                    }
                    $form_state['redirect'] = 'photo_holder/list';
                }
			}
		}
    }
}

// Xoa photo holder
function delete_photo_holder($form,&$form_state,$value) {
    global $base_url;
    $node = node_load($value);
    if($node->type !== 'photo_holder') {
        drupal_not_found('Không tìm thấy trang');
    }
    if($node){
        $form['photo_holder'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        $form['photo_holder']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-10">Bạn có chắc chắn muốn xóa ảnh ? </h3> <a href="'.$base_url.'/photo_holder/list" class="btn btn-info pull-right" style="margin-top:24px;"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['photo_holder']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );
        $form['photo_holder']['body']['name_photoholder'] = array(
            '#type' => 'item',
            '#markup' => '<b>Tên ảnh chuyển đổi : </b>' .$node->title,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );

        $media_path = isset($node->field_file_photo_holder['und'][0]['uri']) ? file_create_url($node->field_file_photo_holder['und'][0]['uri']) : '';

        if($media_path) {
            $media = '<img src="' . $media_path . '" width="100"/>';
        }
        $form['photo_holder']['body']['slide_media'] = array(
            '#type' => 'item',
            '#markup' => $media,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['photo_holder']['body']['id_photoholder'] = array(
            '#type' => 'hidden',
            '#value' => $node->nid,
        );
        
        $form['photo_holder']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );

        $form['photo_holder']['footer']['delete_slide'] = array(
            '#type' => 'submit',
            '#value' => 'Xóa ảnh',
            '#attributes' => array(
                'class' => array('btn','btn-danger')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>'
        );

        return $form;
    }
	
}

// Xoa file anh
function delete_photo_holder_submit($form,&$form_state) {
	$anh_nid = $form_state['values']['id_photoholder'];
	$node = node_load($anh_nid);
    if($node->type == 'photo_holder')  {
        $fid = $node->field_file_photo_holder['und'][0]['fid'];
        $is_delete = TRUE;
        node_delete($anh_nid);
        $file = file_load($fid);
        file_delete($file);
        drupal_set_message('Đã xóa nội dung');
    } else {
        drupal_set_message('Không tìm thấy ảnh','error');
    }
}

// Danh sach anh
function danhsach_photo_holder($form,&$form_state) {
	global $user,$base_url;
	$header = array(
        array('data' => 'STT'),
        array('data' => 'Tên ảnh chuyển đổi'),
        array('data' => 'Ảnh chuyển đổi'),
        array('data' => 'Thao tác'),
    );
    $form = array();
    $rows = danhsach_photo_holder_table();

    $form['photoholder_danhsach'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['photoholder_danhsach']['header'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['photoholder_danhsach']['header']['create_photo'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/photo_holder/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Tạo ảnh chuyển đỏi</a>',
    );
    $form['photoholder_danhsach']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có ảnh',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['photoholder_danhsach']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}

function danhsach_photo_holder_table() {
    global $base_url;
	$limit = 20;
	$query = db_select('node','n')->extend('PagerDefault')->distinct()->limit($limit);
    $query->condition('n.type','photo_holder');

    $query->fields('n',array('nid'));
    $query->orderBy('n.changed','DESC') ;
    $result = $query->execute();
    $rows = array();
    if($result->rowCount()) {
    	$index = 1;
    	foreach($result as $row) {
    		$node = node_load($row->nid);
    		$media = '';
    		$media_filename = '';
    		if(isset($node->field_file_photo_holder['und'][0]['uri'])) {
    			$media_filename = file_create_url($node->field_file_photo_holder['und'][0]['uri']);
    			$media = '<img src="'.$media_filename.'" width="200"/>';
    		}
    		$rows[] = array(
    			$index,
    			$node->title,
    			$media,
                 '<div class="btn-group">
                      <button type="button" class="btn btn-success">Thao tác</button>
                      <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                          <span class="caret"></span>
                          <span class="sr-only">Toggle Dropdown</span>
                      </button>
                      <ul class="dropdown-menu" role="menu">
                          <li><a href="'.$base_url.'/photo_holder/edit/'.$node->nid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                          <li><a href="'.$base_url.'/photo_holder/delete/'.$node->nid.'" ><i class="fa fa-trash"></i> Xóa</a></li>
                      </ul>
                  </div>'
    		);
    	}
    }
    return $rows;
}