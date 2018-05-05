<?php

function list_playlist_form($form , & $form_state){
    global $user,$base_url;

    if(isset($form_state['storage']['rows']) && $form_state['storage']['rows']){
        $rows = $form_state['storage']['rows'];
    }else {
        $rows = load_playlist();
    }
    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Tên Playlist'),
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
        array('data'=>'Số Files'),
        array('data'=>'Thời lượng'),
        array('data'=>'Nhóm nội dung'),
        array('data'=>'Thao tác'),

    );
    //Load Content Dashboard block
    $adtrue_block = module_invoke('adtrue_block', 'block_view', 'content_dashboard');
    $adtrue_block_content = $adtrue_block['content'];
    $form = $adtrue_block_content;

    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['list_broadcast']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>'.t('Danh sách Playlist').'</h3>',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );

    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['search_box'] = array(
        '#type' => 'container',
        '#weight' => 2,
    );
    $vocabulary = taxonomy_vocabulary_machine_name_load('noidung');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    $options[0] = 'Chọn danh mục';
    if($terms){
        foreach($terms as $term){
            $options[$term->tid] = $term->name;
        }
    }
    $form['list_broadcast']['body']['search_box']['playlist_category'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Danh mục'),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4">',
        '#default_value' => isset($form_state['values']['playlist_category']) ? $form_state['values']['playlist_category'] : '',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body']['search_box']['playlist_title'] = array(
        '#type' => 'textfield',
        '#title' => 'Từ khóa',
        '#prefix' => '<div class="col-md-4">',
        '#suffix' => '</div>',
        '#default_value' => isset($form_state['values']['playlist_title']) ? $form_state['values']['playlist_title'] : '',

    );
    $form['list_broadcast']['body']['search_box']['playlist_search'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#attributtes' => array('class'=>array('btn','btn-info')),
        '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body']['search_box']['add_button'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/manage_playlist/create" class="btn btn-success"><i class="fa fa-plus"></i> Tạo Playlist </a>',
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',


    );

    $form['list_broadcast']['body']['result'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
    );
    $form['list_broadcast']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );

    return $form;
}
function list_playlist_form_submit($form , & $form_state){
    global $base_url;
    $values = $form_state['values'];
    $playlist_category = isset($values['playlist_category']) ? $values['playlist_category'] : 0;
    $playlist_title = isset($values['playlist_title']) ? $values['playlist_title']  : '';

    $rows = load_playlist($playlist_category,$playlist_title);
    $form_state['storage']['rows'] = $rows;
    $form_state['rebuild'] = TRUE;
}

function load_playlist($playlist_category=0,$playlist_title='',$number_per_page=50){
    global $base_url,$user;
    $rows = array();

    $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort')->distinct()->limit($number_per_page);
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'playlist');
    $query->leftJoin('field_data_field_playlist_default','fpd','fpd.entity_id=n.nid');
    $query->condition('field_playlist_default_value',2);
    if($playlist_category){
        $query->leftJoin('field_data_field_playlist_category','fpc','fpc.entity_id=n.nid');
        $query->condition('field_playlist_category_tid',$playlist_category);
    }
    if($playlist_title){
        $query->condition('title', '%' . db_like($playlist_title) . '%', 'LIKE');
    }

    //Check role for show the playlist
    if(in_array('marketing staff',$user->roles)){
        $query->condition('n.uid',$user->uid);
    }
    $query->orderBy('n.created', 'DESC');


    $result = $query->execute();
    if ($result->rowCount()) {
        $i = 1;
        foreach ($result as $row) {
            $node = node_load($row->nid);
            //Counting number slides in playlist
            $number_slides = isset($node->field_slides_playlist['und']) ? count($node->field_slides_playlist['und']) : 0;
            //Tính thời lượng playlist
            $slide_ids = isset($node->field_slides_playlist['und']) ? $node->field_slides_playlist['und'] : '';
            $total_time = 0;
            if($slide_ids){
                foreach($slide_ids as $slide_id){
                    foreach($slide_id as $key=>$nid) {
                        if($nid){
                            $slide = node_load($nid);
                            $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;
                        }
                    }
                }
            }

            if (isset($node->field_playlist_category['und'][0]['tid'])) {
                $term = taxonomy_term_load($node->field_playlist_category['und'][0]['tid']);
                $category_name = $term->name;
            } else {
                $category_name = '';
            }
            //Get author information
            $user=user_load($node->uid);
            $rows[] = array(
                $i,
                '<a href="' . $base_url . '/manage_playlist/view/' . $node->nid . '">' . $node->title . '</a>',
                $user->name,
                date('d/m/Y H:i:s',$node->created),
                $number_slides,
                gmdate('H:i:s',$total_time),
                $category_name,
                '<div class="btn-group">
                        <button type="button" class="btn btn-success">Thao tác</button>
                        <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                            <span class="caret"></span>
                            <span class="sr-only">Toggle Dropdown</span>
                        </button>
                        <ul class="dropdown-menu" role="menu">
                            <li><a href="' . $base_url . '/manage_playlist/edit/' . $node->nid . '"><i class="fa fa-edit"></i> Sửa</a></li>
                            <li><a href="' . $base_url . '/manage_playlist/delete/playlist/' . $node->nid . '"><i class="fa fa-trash"></i> Xóa</a></li>
                        </ul>
                    </div>'
            );
            $i++;
        }
    }
    return $rows;
}
function adtrue_playlist_create_form($form, & $form_state){
    $type = isset($_GET['type']) ? $_GET['type'] : 2;

    drupal_add_js(drupal_get_path('module','playlist_management').'/assets/js/filter_table.js');
    global $user,$base_url;

    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['list_broadcast']['header'] = array(
        '#type' => 'item',
        '#markup' =>'<h3>Bước 1 : Nhập thông tin Playlist</h3>',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['col_left'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['col_right'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['col_left']['playlist_name'] = array(
        '#type' => 'textfield',
        '#title' => 'Tên Playlist',
        '#default_value' => (isset($form_state['values']['playlist_name']) ? $form_state['values']['playlist_name'] : ''),
        '#required' => TRUE,
    );
    $form['list_broadcast']['body']['col_left']['playlist_body'] = array(
        '#type' => 'textarea',
        '#title' => 'Mô tả',
    );

    $vocabulary = taxonomy_vocabulary_machine_name_load('noidung');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    if($terms){
        foreach($terms as $term){
            $options[$term->tid] = $term->name;
        }
    }
    $form['list_broadcast']['body']['col_right']['playlist_category'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Danh mục'),
        '#required' => TRUE,

    );
    /*
    $status_playlist = field_info_field('field_status_playlist');
    $status_playlist_values = $status_playlist['settings']['allowed_values'];
    if($status_playlist_values){
        foreach($status_playlist_values as $key => $value){
            $options_status[$key] = $value;
        }
    }
    $form['list_broadcast']['body']['col_right']['playlist_status'] = array(
        '#type' => 'select',
        '#options' => $options_status,
        '#title' => t('Trạng thái'),
        '#required' => TRUE,

    );
    */
    $form['list_broadcast']['type'] = array(
        '#type' => 'hidden',
        '#value' => $type
    );
    $form['list_broadcast']['footer']['next'] = array(
        '#type' => 'submit',
        '#value' =>'Tạo Playlist',
        '#attributes' => array('class'=>array('btn btn-success')),
        '#prefix' => '<div class="box-footer with-border"><div class="col-md-3 pull-right">',
        '#suffix' => '</div></div>'

    );


    return $form;
}


function adtrue_playlist_create_form_submit($form,&$form_state){
    global $user;
    $values = $form_state['values'];

    if($values){
        $node = new stdClass();
        $node->type = 'playlist';
        $node->title = $values['playlist_name'];
        $node->field_playlist_category['und'][0]['tid'] = $values['playlist_category'];
        $node->field_playlist_default['und'][0]['value'] = $values['type'];
        $node->body['und'][0]['value'] = $values['playlist_body'];
        $node->field_status_playlist['und'][0]['value'] = $values['playlist_status'];
        $node->language = LANGUAGE_NONE;
        $node->uid = $user->uid;
        //$node->field_slides_playlist[$node->language][] = array();

        node_save($node);
        if($node->nid){
            //drupal_set_message('Tạo  playlist thành công');

            $form_state['redirect'] = 'manage_playlist/update/' . $node->nid;

        }else{
            drupal_set_message('Có lỗi trong quá trình tạo playlist','error');
            $form_state['redirect'] = 'manage_playlist/list';
        }
    }


}

function load_slides($category=0,$keyword='',$slide_ids=array()){
    $query = db_select('node','n');
    $query->fields('n',array('nid'));

    if($category){
        $query->leftJoin('field_data_field_slide_category_content','scc','scc.entity_id=n.nid');
        $query->condition('field_slide_category_content_tid',$category);
    }
    if($keyword){
        $query->condition('title', '%' . db_like($keyword) . '%', 'LIKE');
    }
    if($slide_ids){
        foreach($slide_ids as $key=>$slide_id){
            $query->condition('n.nid',$slide_id,'<>');
        }
    }

    $query->condition('n.type','slides');
    $query->condition('n.status',1);

    $query->orderBy('n.created','desc');

    $result = $query->execute()->fetchAll();
    $options = array();
    if($result) {
        $i = 1;
        foreach ($result as $row) {
            $slide = node_load($row->nid);

            if(isset($slide->field_media_type['und'][0]['value'])){
                $slide_media = field_info_field('field_media_type');
                $slide_media_values = $slide_media['settings']['allowed_values'];
                $media_type = $slide_media_values[$slide->field_media_type['und'][0]['value']];
            }else{
                $media_type = '';
            }
            if(isset($slide->field_slide_category_content['und'][0]['tid']) && $slide->field_slide_category_content['und'][0]['tid']){
                $term = taxonomy_term_load($slide->field_slide_category_content['und'][0]['tid']);
                $category_name = $term->name;
            }else{
                $category_name = '';
            }
            if(isset($slide->field_link_open_file['und'][0]['value']) && $slide->field_link_open_file['und'][0]['value']) {
                $media_path = $slide->field_link_open_file['und'][0]['value'];
            }else{
                $media_path = file_create_url($slide->field_slide_file['und'][0]['uri']);
            }

            $media = '';
            if(isset($slide->field_media_type['und'][0]['value'])) {
                switch ($slide->field_media_type['und'][0]['value']) {
                    case 1:
                        $media = '<video width="250" controls="controls">
                                        <source src="' . $media_path . '" type="video/mp4" autostart="false">
                                  </video>';
                        break;
                    case 2:
                        break;
                    case 3:
                        $media = '<img src="' . $media_path . '" width="250"/>';
                        break;

                }
            }

            //Get usuername create the slide
            $username = user_load($slide->uid);
            $options[$slide->nid] = array(
                'slide' => $slide->title,
                'slide_media' => $media,
                'slide_duration' => gmdate('H:i:s',$slide->field_slide_duration['und'][0]['value']),
                'category_name' => $category_name,
                'slide_creator' => $username->name,
                'slide_created' => date('Y-m-d', $slide->created)
            );
            $i++;
        }
    }
    return $options;
}
function update_playlist_form($form, &$form_state,$playlist_id){
    global $user,$base_url;
    //Load list slides
    $playlist = node_load($playlist_id);

    $slide_ids = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : '';

    if(isset($form_state['storage']['results'])){
        $options = $form_state['storage']['results'];
    }else{
        $options = load_slides('','',$slide_ids);
    }
    $header =  array(
        'slide' => 'Tên nội dung',
        'slide_media' => 'Nội dung',
        'slide_duration' => 'Thời lượng',
        'category_name' => 'Nhóm Nội dung',
        'slide_creator' => 'Người tạo',
        'slide_created' => 'Ngày tạo'
    );

    $form['playlist_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['playlist_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['playlist_info']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>'.$playlist->title.'</h2>',
        '#prefix' => '<div class="col-md-10">',
        '#suffix' => '</div>'
    );
    $form['playlist_info']['header']['back_playlist'] = array(
        '#type' => 'item',
        '#markup' => '<a href="'.$base_url.'/manage_playlist/edit/'.$playlist_id.'" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>',
        '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>'
    );
    $form['playlist_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['playlist_info']['body']['info'] = array(
        '#type' => 'item',
        '#markup' => isset($playlist->body['und'][0]['value']) ? $playlist->body['und'][0]['value'] : 'Chưa có Mô tả cho playlist. Vui lòng cập nhật!',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['header']['search_box'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );

    $vocabulary = taxonomy_vocabulary_machine_name_load('noidung');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    $opts[0] = 'Chọn danh mục';
    if($terms){
        foreach($terms as $term){
            $opts[$term->tid] = $term->name;
        }
    }
    $form['list_broadcast']['header']['search_box']['slide_category'] = array(
        '#type' => 'select',
        '#title' => 'Danh mục',
        '#options' => $opts,
        '#default_value' => (isset($form_state['values']['slide_category']) ? $form_state['values']['slide_category'] : ''),
        '#prefix' => '<div class="col-md-4">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header']['search_box']['slide_title'] = array(
        '#type' => 'textfield',
        '#title' => 'Nội dung',
        '#default_value' => (isset($form_state['values']['slide_title']) ? $form_state['values']['slide_title'] : ''),
        '#prefix' => '<div class="col-md-5">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header']['search_box']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#attributes' => array(
            'class' => array('btn','btn-success')
        ),
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#submit' => array('search_playlist_submit')

    );
    $form['list_broadcast']['body']['slides'] = array(
        '#weight' => 3,
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $options,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>array('table','table-striped')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['playlist_id'] = array(
        '#type' => 'hidden',
        '#value' => $playlist_id
    );
    $form['list_broadcast']['footer']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Lưu nội dung',
        '#attributes' => array(
            'class' => array('btn','btn-info')
        ),
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#submit' => array('update_playlist_submit')
    );

    return $form;
}
function search_playlist_submit($form,&$form_state){
    $values = $form_state['values'];
    $playlist_id = $values['playlist_id'];
    $playlist = node_load($playlist_id);
    $slide_ids = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : '';
    $options = load_slides($values['slide_category'],$values['slide_title'],$slide_ids);
    $form_state['storage']['results'] = $options;
    $form_state['rebuild'] = true;

}
function update_playlist_submit($form, &$form_state){
    $values = $form_state['values'];

    $playlist_id = $values['playlist_id'];
    $slides = $values['slides'];


    $playlist = node_load($playlist_id);
    $order_slides = array();
    $orders = array();
    if(isset($playlist->field_playlist_orders['und'][0]['value'])){
       $order_slides = isset($playlist->field_playlist_orders['und'][0]['value']) ? json_decode($playlist->field_playlist_orders['und'][0]['value']) : '';
       if($order_slides){
           foreach($order_slides as $key=>$value){
               $orders[$key] = $value;
           }
       }

    }
    if($orders){
        $last_order = max($orders);
    }else{
        $last_order = 0;
    }
    $total_files = isset($playlist->field_playlist_total_files['und'][0]['value']) ? $playlist->field_playlist_total_files['und'][0]['value'] : 0;
    $current_slides = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : 0;
    $current_slides_nid = array();
    if($current_slides){
        foreach($current_slides as $current_slide){
            $current_slides_nid[] = $current_slide['nid'];
        }
    }
    $count = 1;
    foreach($slides as $key=>$value) {
        if($value){
            $playlist->field_slides_playlist['und'][] = array('nid' => $value);
            $orders[$value] = $last_order + $count;
            if(!in_array($value,$current_slides_nid)){
                $total_files++;
            }
            $count++;
        }
    }
    //Tạo unique ID cho từng slide trong playlist

    $playlist->field_playlist_orders['und'][0]['value'] = json_encode($orders);
    $playlist->field_playlist_total_files['und'][0]['value'] = $total_files;
    node_save($playlist);

    drupal_set_message('Cập nhật nội dung Playlist thành công!');
    $form_state['redirect'] = 'manage_playlist/view/'.$playlist_id;

}

function view_playlist_form($form, &$form_state,$playlist_id){
    global $user,$base_url;
    drupal_add_js(drupal_get_path('module','playlist_management').'/assets/videojs/video.js');
    drupal_add_js(drupal_get_path('module','playlist_management').'/assets/js/playlist_management.js');
    drupal_add_css(drupal_get_path('module','playlist_management').'/assets/videojs/video-js.css');

    if($playlist_id){
        $is_used = 0;
        $playlist = node_load($playlist_id);
        //Check playlist is used in broadcast published.
        $query = db_select('node','n');
        $query->condition('n.type','broadcast_playlist_time');
        $query->fields('n',array('nid'));
        $query->fields('b',array('nid'));
        $query->leftJoin('field_data_field_playlist_broadcast_time_id','pbti','pbti.entity_id=n.nid');
        $query->condition('field_playlist_broadcast_time_id_nid',$playlist_id);
        $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpti','bpti.entity_id=n.nid');
        $query->leftJoin('node','b','b.nid=field_broadcast_playlist_time_id_nid');
        $query->condition('b.type','broadcast_playlist');


        $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=b.nid');
        $query->condition('field_approval_playlist_value',2);

        $query_result = $query->execute();
        if($query_result->rowCount()){
            $is_used = 1;
        }
        if($is_used){
            $header =  array(
                array('data'=>'Tên Nội dung'),
                array('data'=>'Nội dung'),
                array('data'=>'Thời lượng'),
                array('data' => 'Nhóm nội dung'),
                array('data' => 'Người tạo'),
                array('data' => 'Ngày tạo'),
                array('data' => 'Sắp xếp'),
            );
        }else{
            $header =  array(
                array('data'=>'Tên Nội dung'),
                array('data'=>'Nội dung'),
                array('data'=>'Thời lượng'),
                array('data' => 'Nhóm nội dung'),
                array('data' => 'Người tạo'),
                array('data' => 'Ngày tạo'),
                array('data' => 'Sắp xếp'),
                array('data' => 'Thao tác')
            );
        }

        $count = 0;
        $rows = array();
        if(isset($playlist->field_slides_playlist['und'])){
            $slide_ids = $playlist->field_slides_playlist['und'];
            $media = '';

            $order_slides = json_decode($playlist->field_playlist_orders['und'][0]['value']);
            $total_time = 0;
            $total_files = 0;
            foreach($slide_ids as $slide_id){
                $slide = node_load($slide_id['nid']);
                //Lấy đường dẫn thực của file

                $media_path = isset($slide->field_link_open_file['und'][0]['value']) ? $slide->field_link_open_file['und'][0]['value'] : file_create_url($slide->field_slide_file['und'][0]['uri']);

                switch ($slide->field_media_type['und'][0]['value']){
                    case 1:
                        $media = '<video width="250" controls="controls">
                                        <source src="'.$media_path.'" type="video/mp4" autostart="false">
                                  </video>';
                        break;
                    case 2:
                        break;
                    case 3:
                        $media = '<img src="'.$media_path.'" width="250"/>';
                        break;

                }
                $order = '';
                foreach ($order_slides as $key=>$value){
                    if($key==$slide_id['nid']){
                        $order = $value;
                    }
                }

                $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;

                //Load user create the slide
                $username = user_load($slide->uid);
                //Load category name
                $terms = taxonomy_term_load($slide->field_slide_category_content['und'][0]['tid']);
                $category_name = isset($terms->name) ? $terms->name : '';
                if($is_used){
                    $rows[$order] = array(
                        $slide->title,
                        $media,
                        gmdate('H:i:s',isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0),
                        $category_name,
                        $username->name,
                        date('d/m/Y',$slide->created),
                        array(
                            'data' => array(
                                '#type' => 'textfield',
                                '#title' => 'Order Slide',
                                '#title_display' => 'invisible',
                                '#value' => $order,
                                '#name' => 'order_slide['.$slide->nid.']',
                                '#size' => 5,
                                '#attributes' => array('disabled' => TRUE),
                            ),
                        )
                    );
                }else{
                    $rows[$order] = array(
                        $slide->title,
                        $media,
                        gmdate('H:i:s',isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0),
                        $category_name,
                        $username->name,
                        date('d/m/Y',$slide->created),
                        array(
                            'data' => array(
                                '#type' => 'textfield',
                                '#title' => 'Order Slide',
                                '#title_display' => 'invisible',
                                '#value' => $order,
                                '#name' => 'order_slide['.$slide->nid.']',
                                '#size' => 5
                            ),
                        ),
                        '<a href="'.$base_url.'/manage_playlist/delete/slide/'.$slide->nid.'/'.$playlist_id.'" class="btn btn-danger"><i class="fa fa-trash-o"></i> Xóa</a>'

                    );
                }

                //Increase count
                $total_files++;

            }
            ksort($rows);
        }

//        $player = '<video id="player" class="video-js vjs-default-skin" controls preload="none" width="480" height="320" data-setup="{}">
//                    <source src="http://vjs.zencdn.net/v/oceans.mp4" type="video/mp4">
//                    <p class="vjs-no-js">To view this video please enable JavaScript, and consider upgrading to a web browser that <a href="http://videojs.com/html5-video-support/" target="_blank">supports HTML5 video</a></p>
//                  </video>';
        $form['playlist_info'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-info')
            ),

        );
        $form['playlist_info']['header'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['playlist_info']['header']['playlist_title'] = array(
            '#type' => 'item',
            '#markup' => '<h3>'.$playlist->title.'</h3>',
            '#prefix' => '<div class="col-md-10">',
            '#suffix' => '</div>'
        );
        $playlist_type = $playlist->field_playlist_default['und'][0]['value'];
        if($playlist_type == 1){
            $back_url = $base_url.'/manage_playlist/default/list';
        }elseif($playlist_type==2){
            $back_url = $base_url.'/manage_playlist/list';
        }else{
            $back_url = $base_url.'/manage_playlist/connect/list';
        }
        $form['playlist_info']['header']['back_playlist'] = array(
            '#type' => 'item',
            '#markup' => '<a href="'.$back_url.'" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>',
        );
        $form['playlist_info']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body with-border">',
            '#suffix' => '</div>'
        );
        $form['playlist_info']['body']['col_left'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['playlist_info']['body']['col_right'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['playlist_info']['body']['col_left']['info'] = array(
            '#type' => 'item',
            '#markup' => isset($playlist->body['und'][0]['value']) ? '<b>Mô tả</b> : '.$playlist->body['und'][0]['value'] : '<b>Mô tả</b> : Chưa có Mô tả cho playlist. Vui lòng cập nhật!',
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['playlist_info']['body']['col_left']['number_slides'] = array(
            '#type' => 'item',
            '#markup' => '<b>Tổng số Nội dung</b> : '.$total_files,
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['playlist_info']['body']['col_left']['total_time'] = array(
            '#type' => 'item',
            '#markup' => '<b>Thời lượng</b> : '.gmdate('H:i:s',$total_time),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );

        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-success')
            )


        );
        if(!$is_used){
            $add_more_slide = '<a href="'.$base_url.'/manage_playlist/update/'.$playlist_id.'" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Thêm nội dung</a>';
        }else{
            $add_more_slide = '';
        }
        $form['list_broadcast']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-10">Danh sách nội dung</h3> '.$add_more_slide,
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );
        /*
        $form['list_broadcast']['body']['playlist_view'] = array(
            '#type' => 'tableselect',
            '#header' => $header,
            '#options' => $rows,
            '#empty' => t('Chưa có nội dung nào.'),
            '#attributes' => array('class'=>array('table','table-striped')),

        );
        */
        $form['list_broadcast']['body']['playlist_view'] = array(
            '#weight' => 3,
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
            '#empty' => 'Chưa có Nội dung nào',
            '#attributes' => array('class'=>'table table-striped'),
        );

        $form['list_broadcast']['playlist_id'] = array(
            '#type' => 'hidden',
            '#value' => $playlist_id,
        );
        if(!$is_used) {
            $form['list_broadcast']['footer'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-footer with-border">',
                '#suffix' => '</div>'
            );

            $form['list_broadcast']['footer']['submit'] = array(
                '#type' => 'submit',
                '#value' => 'Cập nhật sắp xếp',
                '#prefix' => '<div class="pull-right">',
                '#suffix' => '</div>',
                '#submit' => array('order_update_playlist_submit'),
                '#attributes' => array(
                    'class' => array('btn', 'btn-info')
                ),

            );
        }

        return $form;
    }else{
        drupal_set_message('Playlist không tồn tại','warning');
        drupal_goto('manage_playlist/list');
    }

}
function order_update_playlist_submit($form, &$form_state){
    $order_slides = $form_state['input']['order_slide'];
    $playlist_id = $form_state['values']['playlist_id'];
    if($playlist_id){
        $node = node_load($playlist_id);
        $node->field_playlist_orders['und'][0]['value'] = json_encode($order_slides);
        node_save($node);
        $form_state['rebuild'] = TRUE;
        $form_state['redirect'] = 'manage_playlist/view/'.$playlist_id;
    }
}

function delete_slide_playlist_submit($form,&$form_state){
    $values = $form_state['values'];
    $slides = $values['playlist_view'];
    $playlist = node_load($values['playlist_id']);
    $is_deleted = 0;
    $nids = array();
    foreach($slides as $key=>$value){
        if(!$value) {
            $nids[] = array('nid' => $key);
        }
    }

    if($nids){
        $playlist->field_slides_playlist['und'] = $nids;
        node_save($playlist);
        $form_state['rebuild'] = TRUE;

    }

}

function edit_playlist_form($form,&$form_state,$playlist_id){
    global $user, $base_url;
    if($playlist_id){
        $playlist = node_load($playlist_id);
        //Check playlist is used in broadcast published.
        $is_used = 0;
        $query = db_select('node','n');
        $query->condition('n.type','broadcast_playlist_time');
        $query->fields('n',array('nid'));
        $query->fields('b',array('nid'));
        $query->leftJoin('field_data_field_playlist_broadcast_time_id','pbti','pbti.entity_id=n.nid');
        $query->condition('field_playlist_broadcast_time_id_nid',$playlist_id);
        $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpti','bpti.entity_id=n.nid');
        $query->leftJoin('node','b','b.nid=field_broadcast_playlist_time_id_nid');
        $query->condition('b.type','broadcast_playlist');


        $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=b.nid');
        $query->condition('field_approval_playlist_value',2);

        $query_result = $query->execute();
        if($query_result->rowCount()){
            $is_used = 1;
        }

        if($is_used){
            $form['playlist_info'] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#attributes' => array(
                    'class' => array('box', 'box-danger')
                ),
            );
            $form['playlist_info']['header'] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#prefix' => '<div class="box-header with-border">',
                '#suffix' => '</div>'
            );

            $form['playlist_info']['header']['header_title'] = array(
                '#type' => 'item',
                '#markup' => '<h3>Bạn không thể sửa playlist đã có trong lịch phát sóng được phê duyệt!</h3>'
            );

            $form['playlist_info']['body'] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#prefix' => '<div class="box-body">',
                '#suffix' => '</div>'
            );
            $form['playlist_info']['body']['playlist_name'] = array(
                '#type' => 'item',
                '#markup' => '<b>Tên Playlist : </b>' . $playlist->title,
                '#weight' => 2
            );
            $form['playlist_info']['footer'] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#prefix' => '<div class="box-footer with-border">',
                '#suffix' => '</div>'
            );
            $form['playlist_info']['footer']['return_button'] = array(
                '#type' => 'item',
                '#markup' => '<a href="' . $base_url . '/manage_playlist/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>',
                '#weight' => 2
            );

        }else {
            $form['playlist_info'] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#attributes' => array(
                    'class' => array('box', 'box-info')
                ),
            );
            $form['playlist_info']['header'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-header with-border">',
                '#suffix' => '</div>'
            );

            $form['playlist_info']['header']['header_title'] = array(
                '#type' => 'item',
                '#markup' => '<h3 class="col-md-10">Thông tin chung</h3> <a href="' . $base_url . '/manage_playlist/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>'
            );

            $form['playlist_info']['body'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-body">',
                '#suffix' => '</div>'
            );
            $form['playlist_info']['body']['col_left'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="col-md-6">',
                '#suffix' => '</div>'
            );
            $form['playlist_info']['body']['col_right'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="col-md-6">',
                '#suffix' => '</div>'
            );
            $form['playlist_info']['body']['col_left']['playlist_name'] = array(
                '#type' => 'textfield',
                '#title' => 'Tên Playlist',
                '#value' => $playlist->title,
                '#required' => TRUE,
            );
            $form['playlist_info']['body']['col_left']['playlist_body'] = array(
                '#type' => 'textarea',
                '#title' => 'Mô tả',
                '#value' => isset($playlist->body['und'][0]['value']) ? $playlist->body['und'][0]['value'] : '',
            );

            $vocabulary = taxonomy_vocabulary_machine_name_load('noidung');
            $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
            if ($terms) {
                foreach ($terms as $term) {
                    $options[$term->tid] = $term->name;
                }
            }
            $form['playlist_info']['body']['col_right']['playlist_category'] = array(
                '#type' => 'select',
                '#options' => $options,
                '#default_value' => (isset($playlist->field_playlist_category['und'][0]['tid']) ? $playlist->field_playlist_category['und'][0]['tid'] : ''),
                '#title' => t('Danh mục Nội dung'),
                '#required' => TRUE,

            );

            /*
            $status_playlist = field_info_field('field_status_playlist');
            $status_playlist_values = $status_playlist['settings']['allowed_values'];

            if($status_playlist_values){
                foreach($status_playlist_values as $key => $value){
                    $options_status[$key] = $value;
                }
            }



            $form['playlist_info']['body']['col_right']['playlist_status'] = array(
                '#type' => 'select',
                '#options' => $options_status,
                '#title' => t('Trạng thái'),
                '#required' => TRUE,
                '#default_value' => $playlist->field_status_playlist['und'][0]['value'],

            );
            */


            $form['list_broadcast'] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#attributes' => array(
                    'class' => array('box', 'box-success')
                ),

            );
            if ($is_used) {
                $add_more_slide = '';
            } else {
                $add_more_slide = '<a href="' . $base_url . '/manage_playlist/update/' . $playlist_id . '" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Thêm nội dung</a>';
            }
            $form['list_broadcast']['header'] = array(
                '#type' => 'item',
                '#markup' => '<h3 class="col-md-10">Danh sách nội dung</h3> ' . $add_more_slide,
                '#prefix' => '<div class="box-header with-border">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-body">',
                '#suffix' => '</div>'
            );

            //Load slides
            $playlist = node_load($playlist_id);

            $header = array(
                array('data' => 'Tên Nội dung'),
                array('data' => 'Nội dung'),
                array('data' => 'Thời lượng'),
                array('data' => 'Nhóm Nội dung'),
                array('data' => 'Người tạo'),
                array('data' => 'Ngày tạo'),
                array('data' => 'Sắp xếp'),
                array('data' => 'Thao tác')
            );

            $rows = array();
            if (isset($playlist->field_slides_playlist['und'])) {
                $slide_ids = $playlist->field_slides_playlist['und'];
                $rows = array();
                $media = '';
                $count = 1;
                $order_slides = array();
                $total_time = 0;

                if (isset($playlist->field_playlist_orders['und'][0]['value'])) {
                    $order_slides = json_decode($playlist->field_playlist_orders['und'][0]['value']);
                }


                foreach ($slide_ids as $slide_id) {
                    $slide = node_load($slide_id['nid']);
                    $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;
                    //Lấy đường dẫn thực của file

                    $media_path = isset($slide->field_link_open_file['und'][0]['value']) ? $slide->field_link_open_file['und'][0]['value'] : file_create_url($slide->field_slide_file['und'][0]['uri']);

                    switch ($slide->field_media_type['und'][0]['value']) {
                        case 1:
                            $media = '<video width="250" controls="controls">
                                        <source src="' . $media_path . '" type="video/mp4" autostart="false">
                                  </video>';
                            break;
                        case 2:
                            break;
                        case 3:
                            $media = '<img src="' . $media_path . '" width="250"/>';
                            break;

                    }
                    $order = '';
                    foreach ($order_slides as $key => $value) {
                        if ($key == $slide_id['nid']) {
                            $order = $value;
                        }
                    }
                    //Load category name
                    $term = taxonomy_term_load($slide->field_slide_category_content['und'][0]['tid']);
                    $category_name = isset($term->name) ? $term->name : '';
                    //Load user create slide
                    $username = user_load($slide->uid);

                    $rows[$order] = array(
                        $slide->title,
                        $media,
                        gmdate('H:i:s', $total_time),
                        $category_name,
                        $username->name,
                        date('d/m/Y', $slide->created),
                        array(
                            'data' => array(
                                '#type' => 'textfield',
                                '#title' => 'Order Slide',
                                '#title_display' => 'invisible',
                                '#value' => $order,
                                '#name' => 'order_slide[' . $slide->nid . ']',
                                '#size' => 5
                            ),
                        ),
                        '<a href="' . $base_url . '/manage_playlist/delete/slide/' . $slide->nid . '/' . $playlist_id . '" class="btn btn-danger"><i class="fa fa-trash-o"></i> Xóa</a>'
                    );


                    $count++;
                }
                ksort($rows);
            }

            /*
            $form['list_broadcast']['body']['playlist_view'] = array(
                '#type' => 'tableselect',
                '#header' => $header,
                '#options' => $rows,
                '#empty' => t('Chưa có nội dung nào.'),
                '#attributes' => array('class'=>array('table','table-striped')),

            );
            */
            $form['list_broadcast']['body']['playlist_view'] = array(
                '#weight' => 3,
                '#theme' => 'table',
                '#header' => $header,
                '#rows' => $rows,
                '#empty' => 'Chưa có Nội dung nào',
                '#attributes' => array('class' => 'table table-striped'),
                '#prefix' => '<div class="box-body">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['footer'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-footer with-border">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['playlist_id'] = array(
                '#type' => 'hidden',
                '#value' => $playlist_id,
            );

            $form['list_broadcast']['footer']['submit'] = array(
                '#type' => 'submit',
                '#value' => 'Cập nhật Playlist',
                '#prefix' => '<div class="pull-right">',
                '#suffix' => '</div>',
                '#attributes' => array(
                    'class' => array('btn', 'btn-info')
                ),
                '#submit' => array('edit_playlist_form_submit')


            );
        }
        return $form;
    }else{
        drupal_goto('manage_playlist/list');
    }
}

function edit_playlist_form_submit($form,&$form_state){
    $input = $form_state['input'];
    $playlist_id = $input['playlist_id'];
    $order_sldies = isset($input['order_slide']) ? json_encode($input['order_slide']) : '';


    if($playlist_id){
        $playlist = node_load($playlist_id);
        $total_files = isset($playlist->field_slides_playlist['und']) ? count($playlist->field_slides_playlist['und']) : 0;
        $current_total_files = isset($playlist->field_playlist_total_files['und'][0]['value']) ? $playlist->field_playlist_total_files['und'][0]['value'] : 0;

        $playlist->title = $input['playlist_name'];
        $playlist->body['und'][0]['value'] = $input['playlist_body'];
        $playlist->field_playlist_category['und'][0]['tid'] = $input['playlist_category'];
        $playlist->field_playlist_orders['und'][0]['value'] = $order_sldies;
        $playlist->field_status_playlist['und'][0]['value'] = isset($input['playlist_status']) ? $input['playlist_status'] : 0;
        if(!$current_total_files){
            $playlist->field_playlist_total_files['und'][0]['value'] = $total_files;
        }
        node_save($playlist);
        drupal_set_message('Cập nhật Playlist thành công!');
        $playlist_type = $playlist->field_playlist_default['und'][0]['value'];
        if($playlist_type == 1){
            $form_state['redirect'] = 'manage_playlist/default/list';
        }elseif($playlist_type==2){
            $form_state['redirect'] = 'manage_playlist/list';
        }else{
            $form_state['redirect'] = 'manage_playlist/connect/list';
        }

    }

}


function load_slides_by_playlist_id($playlist_id){
    $playlist = node_load($playlist_id);
    $slide_ids = $playlist->field_slides_playlist['und'];

    foreach($slide_ids as $slide_id) {
        $slide = node_load($slide_id['nid']);
        //Lấy đường dẫn thực của file
        $media_path = file_create_url($slide->field_slide_file['und'][0]['uri']);

        switch ($slide->field_media_type['und'][0]['value']) {
            case 1:
                $media = '<video width="250" controls="controls">
                                        <source src="' . $media_path . '" type="video/mp4" autostart="false">
                                  </video>';
                break;
            case 2:
                break;
            case 3:
                $media = '<img src="' . $media_path . '" width="250"/>';
                break;

        }
        $order = '';
        $order_slides = json_decode($playlist->field_playlist_orders['und'][0]['value']);
        foreach ($order_slides as $key => $value) {
            if ($key == $slide_id['nid']) {
                $order = $value;
            }
        }
        $rows[$slide->nid] = array(
            'slide_name' => $slide->title,
            'slide' => $media,
            'order_slide' => array(
                'data' => array(
                    '#type' => 'textfield',
                    '#title' => 'Order Slide',
                    '#title_display' => 'invisible',
                    '#value' => $order,
                    '#name' => 'order_slide[' . $slide->nid . ']',
                    '#size' => 5
                ),
            )

        );
    }
    return $rows;
}

function delete_slide_playlist_form($form, &$form_state,$slide_id,$playlist_id){
    global $base_url;
    if($slide_id){
        $slide = node_load($slide_id);
        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        $form['list_broadcast']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-10">Bạn có chắc chắn muốn xóa nội dung ? </h3> <a href="'.$base_url.'/manage_playlist/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['slide_title'] = array(
            '#type' => 'item',
            '#markup' => '<b>Tên Nội dung : </b>' .$slide->title,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $media_path = isset($slide->field_slide_file['und'][0]['uri']) ? file_create_url($slide->field_slide_file['und'][0]['uri']) : '';

        switch ($slide->field_media_type['und'][0]['value']) {
            case 1:
                $media = '<video width="250" controls="controls">
                                <source src="' . $media_path . '" type="video/mp4" autostart="false">
                          </video>';
                break;
            case 2:
                break;
            case 3:
                $media = '<img src="' . $media_path . '" width="250"/>';
                break;

        }
        $form['list_broadcast']['body']['slide_media'] = array(
            '#type' => 'item',
            '#markup' => $media,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['slide_id'] = array(
            '#type' => 'hidden',
            '#value' => $slide_id
        );
        $form['list_broadcast']['playlist_id'] = array(
            '#type' => 'hidden',
            '#value' => $playlist_id
        );
        $form['list_broadcast']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['footer']['delete_slide'] = array(
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

function delete_slide_playlist_form_submit($form, &$form_state){
    $values = $form_state['values'];
    $playlist = node_load($values['playlist_id']);
    $slides = $playlist->field_slides_playlist['und'];
    $new_slides = array();
    foreach($slides as $slide){
        foreach($slide as $key=>$slide_id) {
            if ($slide_id != $values['slide_id']) {
                $new_slides[] = array('nid' => $slide_id);
            }
        }
    }
    $playlist->field_slides_playlist['und'] = $new_slides;

    node_save($playlist);
    $form_state['redirect'] = 'manage_playlist/edit/'.$values['playlist_id'];

}

function delete_playlist_form($form,&$form_state,$playlist_id){
    global $base_url;
    if($playlist_id){
        $playlist = node_load($playlist_id);
        if(isset($playlist->field_slides_playlist['und'])){
            $number_slides = count($playlist->field_slides_playlist['und']);
        }else{
            $number_slides = 0;
        }
        //Check playlist is used in broadcast published.
        $is_used = 0;
        $query = db_select('node','n');
        $query->condition('n.type','broadcast_playlist_time');
        $query->fields('n',array('nid'));
        $query->fields('b',array('nid'));
        $query->leftJoin('field_data_field_playlist_broadcast_time_id','pbti','pbti.entity_id=n.nid');
        $query->condition('field_playlist_broadcast_time_id_nid',$playlist_id);
        $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpti','bpti.entity_id=n.nid');
        $query->leftJoin('node','b','b.nid=field_broadcast_playlist_time_id_nid');
        $query->condition('b.type','broadcast_playlist');


        $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=b.nid');
        $query->condition('field_approval_playlist_value',2);

        $query_result = $query->execute();
        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        if($query_result->rowCount()){
            $is_used = 1;
        }
        if($is_used){
            $form['list_broadcast']['header'] = array(
                '#type' => 'item',
                '#markup' =>'<h3>Bạn không thể xóa Playlist này vì Playlist đang được sử dụng trong lịch phát sóng ! </h3> ',
                '#prefix' => '<div class="box-header with-border">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-body">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body']['slide_title'] = array(
                '#type' => 'item',
                '#markup' => '<b>Tên Playlist : </b>' .$playlist->title,
                '#prefix' => '<div class="col-md-12">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body']['number_slides'] = array(
                '#type' => 'item',
                '#markup' => '<b>Tổng số nội dung : </b>' .$number_slides,
                '#prefix' => '<div class="col-md-12">',
                '#suffix' => '</div>'
            );


            $form['list_broadcast']['playlist_id'] = array(
                '#type' => 'hidden',
                '#value' => $playlist_id
            );
            $form['list_broadcast']['footer'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-footer width-border">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['footer']['return_playlist'] = array(
                '#type' => 'item',
                '#markup' => '<a href="'.$base_url.'/manage_playlist/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại </a>',
                '#prefix' => '<div class="pull-right">',
                '#suffix' => '</div>'
            );
        }else{
            $form['list_broadcast']['header'] = array(
                '#type' => 'item',
                '#markup' =>'<h3 class="col-md-10">Bạn có chắc chắn muốn xóa nội dung ? </h3> <a href="'.$base_url.'/manage_playlist/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại </a>',
                '#prefix' => '<div class="box-header with-border">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-body">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body']['slide_title'] = array(
                '#type' => 'item',
                '#markup' => '<b>Tên Playlist : </b>' .$playlist->title,
                '#prefix' => '<div class="col-md-12">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['body']['number_slides'] = array(
                '#type' => 'item',
                '#markup' => '<b>Tổng số nội dung : </b>' .$number_slides,
                '#prefix' => '<div class="col-md-12">',
                '#suffix' => '</div>'
            );


            $form['list_broadcast']['playlist_id'] = array(
                '#type' => 'hidden',
                '#value' => $playlist_id
            );
            $form['list_broadcast']['footer'] = array(
                '#type' => 'container',
                '#prefix' => '<div class="box-footer width-border">',
                '#suffix' => '</div>'
            );
            $form['list_broadcast']['footer']['delete_playlist'] = array(
                '#type' => 'submit',
                '#value' => 'Xóa Playlist',
                '#attributes' => array(
                    'class' => array('btn','btn-danger')
                ),
                '#prefix' => '<div class="col-md-2 pull-right">',
                '#suffix' => '</div>'
            );

        }



        return $form;
    }
}

function delete_playlist_form_submit($form,&$form_state){
    $values = $form_state['values'];
    $playlist = node_load($values['playlist_id']);
    $playlist_type = $playlist->field_playlist_default['und'][0]['value'];
    if($playlist_type == 1){
        $back_url = '/manage_playlist/default/list';
    }elseif($playlist_type==2){
        $back_url = '/manage_playlist/list';
    }else{
        $back_url = '/manage_playlist/connect/list';
    }

    node_delete($values['playlist_id']);
    drupal_set_message('Xóa Playlist thành công !');

    $form_state['redirect'] = $back_url;
}

function list_playlist_default_form($form,&$form_state){
    global $user,$base_url;

    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Tên Playlist'),
        array('data'=>'Số Files'),
        array('data'=>'Trạng thái'),
        array('data'=>'Danh mục'),
        array('data'=>'Thao tác'),
    );

    $rows = array();
    if(isset($form_state['storage']['rows']) && $form_state['storage']['rows']){
        $rows = $form_state['storage']['rows'];
    } else {
        $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit(20);
        $query->fields('n', array('nid'));
        $query->leftJoin('field_data_field_playlist_default','fpd','fpd.entity_id=n.nid');
        $query->condition('field_playlist_default_value',1);
        $query->condition('n.type', 'playlist', '=');
        $query->orderBy('n.created', 'DESC');

        $result = $query->execute();
        if ($result->rowCount()) {
            $i = 1;
            foreach ($result as $row) {
                $node = node_load($row->nid);
                //Counting number slides in playlist
                $number_slides = isset($node->field_slides_playlist['und']) ? count($node->field_slides_playlist['und']) : 0;

                $status_playlist = field_info_field('field_status_playlist');
                $status_playlist_values = $status_playlist['settings']['allowed_values'];

                $status = $status_playlist_values[$node->field_status_playlist['und'][0]['value']];
                if (isset($node->field_playlist_category['und'][0]['tid'])) {
                    $term = taxonomy_term_load($node->field_playlist_category['und'][0]['tid']);
                    $category_name = $term->name;
                } else {
                    $category_name = '';
                }

                $rows[] = array(
                    $i,
                    '<a href="' . $base_url . '/manage_playlist/view/' . $node->nid . '">' . $node->title . '</a>',
                    $number_slides,
                    $status,
                    $category_name,
                    '<ul class="tools">
                    <li><a href="' . $base_url . '/manage_playlist/edit/' . $node->nid . '" class="btn btn-warning"><i class="fa fa-edit"></i> Sửa</a></li>
                    <li><a href="' . $base_url . '/manage_playlist/delete/playlist/' . $node->nid . '" class="btn btn-danger"><i class="fa fa-remove"></i> Xóa</a></li>
                </ul>'
                );
                $i++;
            }
        }
    }

    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['header']['search_box'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $vocabulary = taxonomy_vocabulary_machine_name_load('noidung');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    $options[0] = 'Chọn danh mục';
    if($terms){
        foreach($terms as $term){
            $options[$term->tid] = $term->name;
        }
    }
    $form['list_broadcast']['header']['search_box']['playlist_category'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Danh mục'),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4">',
        '#default_value' => isset($form_state['values']['playlist_category']) ? $form_state['values']['playlist_category'] : '',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['header']['search_box']['playlist_title'] = array(
        '#type' => 'textfield',
        '#title' => 'Từ khóa',
        '#prefix' => '<div class="col-md-4">',
        '#suffix' => '</div>',
        '#default_value' => isset($form_state['values']['playlist_title']) ? $form_state['values']['playlist_title'] : '',

    );
    $form['list_broadcast']['header']['search_box']['playlist_search'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#attributtes' => array('class'=>array('btn','btn-info')),
        '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['header']['search_box']['add_button'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/manage_playlist/create?type=1" class="btn btn-success"><i class="fa fa-plus"></i> Tạo Playlist  Default</a>',
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',


    );
    $form['list_broadcast']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );

    return $form;
}

function list_playlist_check_internet_form($form, & $form_state){
    global $user,$base_url;

    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Tên Playlist'),
        array('data'=>'Số Files'),
        array('data'=>'Trạng thái'),
        array('data'=>'Danh mục'),
        array('data'=>'Thao tác'),

    );
    $rows = array();
    if(isset($form_state['storage']['rows']) && $form_state['storage']['rows']){
        $rows = $form_state['storage']['rows'];
    }else {
        $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit(20);
        $query->fields('n', array('nid'));
        $query->leftJoin('field_data_field_playlist_default','fpd','fpd.entity_id=n.nid');
        $query->condition('field_playlist_default_value',3);
        $query->condition('n.type', 'playlist', '=');
        $query->orderBy('n.created', 'DESC');



        $result = $query->execute();
        if ($result->rowCount()) {
            $i = 1;
            foreach ($result as $row) {
                $node = node_load($row->nid);
                //Counting number slides in playlist
                $number_slides = isset($node->field_slides_playlist['und']) ? count($node->field_slides_playlist['und']) : 0;

                $status_playlist = field_info_field('field_status_playlist');
                $status_playlist_values = $status_playlist['settings']['allowed_values'];

                $status = $status_playlist_values[$node->field_status_playlist['und'][0]['value']];
                if (isset($node->field_playlist_category['und'][0]['tid'])) {
                    $term = taxonomy_term_load($node->field_playlist_category['und'][0]['tid']);
                    $category_name = $term->name;
                } else {
                    $category_name = '';
                }

                $rows[] = array(
                    $i,
                    '<a href="' . $base_url . '/manage_playlist/view/' . $node->nid . '">' . $node->title . '</a>',
                    $number_slides,
                    $status,
                    $category_name,
                    '<ul class="tools">
                    <li><a href="' . $base_url . '/manage_playlist/edit/' . $node->nid . '" class="btn btn-warning"><i class="fa fa-edit"></i> Sửa</a></li>
                    <li><a href="' . $base_url . '/manage_playlist/delete/playlist/' . $node->nid . '" class="btn btn-danger"><i class="fa fa-remove"></i> Xóa</a></li>
                </ul>'
                );
                $i++;
            }
        }
    }
    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['header']['search_box'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $vocabulary = taxonomy_vocabulary_machine_name_load('noidung');
    $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));
    $options[0] = 'Chọn danh mục';
    if($terms){
        foreach($terms as $term){
            $options[$term->tid] = $term->name;
        }
    }
    $form['list_broadcast']['header']['search_box']['playlist_category'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#title' => t('Danh mục'),
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-4">',
        '#default_value' => isset($form_state['values']['playlist_category']) ? $form_state['values']['playlist_category'] : '',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['header']['search_box']['playlist_title'] = array(
        '#type' => 'textfield',
        '#title' => 'Từ khóa',
        '#prefix' => '<div class="col-md-4">',
        '#suffix' => '</div>',
        '#default_value' => isset($form_state['values']['playlist_title']) ? $form_state['values']['playlist_title'] : '',

    );
    $form['list_broadcast']['header']['search_box']['playlist_search'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#attributtes' => array('class'=>array('btn','btn-info')),
        '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['header']['search_box']['add_button'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/manage_playlist/create?type=3" class="btn btn-success"><i class="fa fa-plus"></i> Tạo Playlist Kiểm tra</a>',
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',


    );
    $form['list_broadcast']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );

    return $form;
}