<?php
/*
* List all category content
*/
function category_content_in_show($form,&$form_state) {
	$form = array();
	$rows = show_category_content_list();
	return base_form_category_content_table($rows);
}

// View danh muc
function category_content_in_list_manage($form,&$form_state) {
	$rows = show_category_content_list(1);
	return base_form_category_content_table($rows);
}


function base_form_category_content_table($rows) {
    global $base_url;
	$form['list_cat_content'] =  array(
		'#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
	);
	$form['list_cat_content']['header'] = array(
		'#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
	);
	$form['list_cat_content']['body'] = array(
		'#type' => 'container',
        '#weight' => 3,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
	);
	$form['list_cat_content']['footer'] = array(
		'#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
	);
	$form['list_cat_content']['header']['cat_create'] = array(
		'#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/danhmuc/noidung/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Thêm danh mục </a>',
	);
	$header  = array(
		array('data' => 'Thứ tự'),
		array('data' => 'Tên danh mục'),
		array('data' => 'Chú thích'),
		array('data' => 'Thao tác')
	);
	$form['list_cat_content']['body']['list'] = array(
		'#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('class'=>array('table table-striped')),
        '#empty' => t('Không có thiết bị nào.'),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
	);
	return $form;
}

// List all result
function show_category_content_list($action=0) {
    global $base_url;
	$limit = 10;
	$voca = taxonomy_vocabulary_machine_name_load('noidung');
    $vid = $voca->vid;
    $terms = taxonomy_term_load_multiple(array(),array('vid' => $vid));
    $rows = array();
    $i = 1;
    foreach($terms as $term) {
    	if($action) {
    		$rows[] = array(
	    		$i,
	    		$term->name,
	    		$term->description,
	    		'<ul class="tools">
                    <li><a href="'.$base_url.'/danhmuc/noidung/edit/'.$term->tid.'" class="btn btn-warning"><i class="fa fa-edit"></i>Sửa</a></li>
                    <li><a href="'.$base_url.'/danhmuc/noidung/delete/'.$term->tid.'" class="btn btn-danger"><i class="fa fa-trash"></i>Xóa</a></li>
                </ul>'
	    	);
    	} else {
    		$rows[] = array(
	    		$i,
	    		$term->name,
	    		$term->description,
	    	);
    	}
    }
    return $rows;
}

// Xay dung form cho edit va adding
function form_base_manage_category_content($tid = '') {
	$form = array();
	$name = '';
	$description = '';
	if($tid) {
		$term = taxonomy_term_load($tid);
		$name = $term->name;
		$description = $term->description;
	}
	$form['cat_content_id'] = array(
		'#type' => 'hidden',
        '#title' => 'tmp file',
        '#default_value' => $tid,
	);
	$form['category_content'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
	$form['category_content']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['category_content']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['category_content']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['category_content']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Chi tiết danh mục</h3>'
    );
    $form['category_content']['body']['name_cat'] = array(
    	'#type' => 'textfield',
        '#title' => t('Tên Danh Mục'),
        '#size' => 60,
        '#default_value' => $name,
        '#maxlength' => 128,
        '#required' => TRUE,
    );
    $form['category_content']['body']['description_cat'] = array(
    	'#type' => 'textarea',
        '#title' => t('Mô tả'),
        '#description' => 'Phần chú thích cho từng danh mục',
        '#default_value' => $description,
        '#size' => 60,
    );
    $form['category_content']['footer']['submit'] = array(
    	'#type' => 'submit',
        '#value' => 'Lưu lại',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-success')
        ),
    );
    $form['#submit'][] = 'submit_category_content_request';
    $form['#validate'][] = 'validate_category_content_request';
    return $form;
}

//Edit noidung category
function category_content_in_editing($form,&$form_state,$value) {
	$voca = taxonomy_vocabulary_machine_name_load('noidung');
    $vid = $voca->vid;
    $term = taxonomy_term_load($value);
    // Kiem tra xem co phai la taxonomy category khong
    if($term->vid !== $vid) {
    	drupal_not_found('Không tìm thấy trang');
    }
	$form  = form_base_manage_category_content($value);

	return $form;
}

function  category_content_in_adding($form,&$form_state) {
	$form  = form_base_manage_category_content();
	return $form;
}

//validate function
function validate_category_content_request($form,&$form_state) {
	$name = $form_state['values']['name_cat'];
	$description = $form_state['values']['description_cat'];
	if(!$name) {
		form_set_error('name_cat','Tên không được trống');
	}
}
// Submit function 
function submit_category_content_request($form,&$form_state) {
	$tid = $form_state['values']['cat_content_id'];
	$name = $form_state['values']['name_cat'];
	$description = $form_state['values']['description_cat'];
	// lay category content
	$voca = taxonomy_vocabulary_machine_name_load('noidung');
    $vid = $voca->vid;

	if($tid) {
		$term = taxonomy_term_load($tid);
		$term->vid = $vid;
		$term->name = $name;
		$term->description = $description;
		taxonomy_term_save($term);
		drupal_set_message('Cập nhật thành công danh mục');
		$form_state['redirect'] = 'danhmuc/noidung/list';

	} else {
		$term = new stdClass();
		$term->vid = $vid;
		$term->name = $name;
		$term->description = $description;
		taxonomy_term_save($term);
		if($term->tid) {
			drupal_set_message('Đã tạo 1 danh mục');
		} else {
			drupal_set_message('Không thể tạo danh mục','error');
		}
		$form_state['redirect'] = 'danhmuc/noidung/list';
	}
}

// Deleting
function category_content_in_deleting($form,&$form_state,$value) {
	global $base_url;
	// lay category content
	$voca = taxonomy_vocabulary_machine_name_load('noidung');
    $vid = $voca->vid;

    $term = taxonomy_term_load($value);
    if($term->vid !== $vid) {
    	drupal_not_found('Không tìm thấy danh mục');
    }
    if($term){
        $form['category_content'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        $form['category_content']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-10">Bạn có chắc chắn muốn xóa danh mục ? </h3> <a href="'.$base_url.'/danhmuc/noidung/list" class="btn btn-info pull-right" style="margin-top:24px;"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['category_content']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );
        $form['category_content']['body']['name_cat'] = array(
            '#type' => 'item',
            '#markup' => '<b>Tên danh mục : </b>' .$term->name,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['category_content']['body']['id_cat'] = array(
            '#type' => 'hidden',
            '#value' => $term->tid,
        );
        
        $form['category_content']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );
        $form['category_content']['footer']['delete_slide'] = array(
            '#type' => 'submit',
            '#value' => 'Xóa danh mục',
            '#attributes' => array(
                'class' => array('btn','btn-danger')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>'
        );

        return $form;
    }
}

// Submit deleting
function category_content_in_deleting_submit($form,&$form_state) {
    global $base_url;
	$tid = $form_state['values']['id_cat'];
	taxonomy_term_delete($tid);
	drupal_set_message('Xóa thành công cửa hàng');
	$form_state['redirect'] = $base_url.'/danhmuc/noidung/list';
}