<?php
/**
 * Created by PhpStorm.
 * User: tuanna0703
 * Date: 1/9/2017
 * Time: 5:11 PM
 */

function broadcast_list_form($form, &$form_state){
    global $user,$base_url;
    $form = array();
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $store_id = isset($_GET['store_id']) ? $_GET['store_id'] : 0;
    $status = isset($_GET['status']) ? $_GET['status'] : 0;
    $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : 0;
    $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;
    $rows = array();


    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Từ ngày/Tới ngày'),
        array('data'=>'Lịch phát sóng'),
        array('data' => 'Ngày còn lại'),
        array('data' => 'Trang thái'),
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
        array('data' => 'Công ty'),
        array('data' => 'Vùng miền'),
        array('data' => 'Tỉnh/Thành'),
        array('data' => 'Cửa hàng'),
        array('data'=>'Thao tác'),

    );
    $number_per_page = 50;
    $result = list_broadcast($company_id,$region_id,$province_id,$store_id,$status,$start_date,$end_date,$header,$number_per_page,$page);
    $rows = $result['rows'];
    $total_result = $result['total_result'];

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
        '#markup' =>'<h3 class="col-md-10">Danh sách Lịch phát sóng</h3>',

    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );


    $company_options = load_voca_info('company');
    $form['list_broadcast']['body']['broadcast_area']['broadcast_company'] = array(
        '#type' => 'select',
        '#title' => 'Công ty',
        '#options' => $company_options,
        '#default_value' => $company_id,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#ajax' => array(
            'callback' => 'region_select_callback',
            'wrapper' => 'region-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
        '#validated' => TRUE,
    );

    if($company_id){
        $region_options = load_voca_info('region',array('company_id'=>$company_id));
    }else{
        $region_options = array(0=>t('Chọn vùng miền'));
    }
    $form['list_broadcast']['body']['broadcast_area']['broadcast_region'] = array(
        '#type' => 'select',
        '#title' => 'Vùng miền',
        '#options' => $region_options,
        '#default_value' => $region_id,
        '#prefix' => '<div id="region-wrapper" class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'province_select_callback',
            'wrapper' => 'province-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );
    if($region_id){
        $province_options = load_voca_info('province',array('company_id'=>$company_id,'region_id'=>$region_id));
    }else{
        $province_options = array(0=>('Chọn tỉnh/thành'));
    }
    $form['list_broadcast']['body']['broadcast_area']['broadcast_province'] = array(
        '#type' => 'select',
        '#title' => 'Tỉnh / Thành',
        '#options' => $province_options,
        '#default_value' => $province_id,
        '#prefix' => '<div id="province-wrapper" class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'store_select_callback',
            'wrapper' => 'store-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );

    if($province_id){
        $store_options = load_voca_info('store',array('company_id'=>$company_id,'region_id'=>$region_id,'province_id'=>$province_id));
    }else{
        $store_options = array(0=>'Chọn cửa hàng');
    }

    $form['list_broadcast']['body']['broadcast_area']['broadcast_store'] = array(
        '#type' => 'select',
        '#title' => 'Cửa hàng',
        '#options' => $store_options,
        '#default_value' => $store_id,
        '#prefix' => '<div class="col-md-3" id="store-wrapper">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );


    $form['list_broadcast']['body']['broadcast_area']['start_date'] = array(
        '#type' => 'date_popup',
        //'#timepicker' => 'timepicker',
        '#title' => 'Từ ngày',
        '#date_label_position' => 'none',
        '#date_format' => 'd/m/Y',
        '#default_value' => $start_date,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#size' => 30
    );
    $form['list_broadcast']['body']['broadcast_area']['end_date'] = array(
        '#type' => 'date_popup',
        //'#timepicker' => 'timepicker',
        '#title' => 'Tới ngày',
        '#date_label_position' => 'none',
        '#date_format' => 'd/m/Y',
        '#default_value' => $end_date,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#size' => 30
    );

    $status_broadcast = field_info_field('field_approval_playlist');
    $status_broadcast_values = $status_broadcast['settings']['allowed_values'];
    $review_options = array('0'=> 'Chọn trạng thái');
    if($status_broadcast_values){
        foreach($status_broadcast_values as $key=>$value){
            $review_options[$key] = $value;
        }
    }
    $form['list_broadcast']['body']['broadcast_area']['status'] = array(
        '#type' => 'select',
        '#title' => 'Trạng thái',
        '#options' => $review_options,
        '#default_value' => $status,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,

    );

    $form['list_broadcast']['body']['buttons'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12" style="padding-right:0px;">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body']['buttons']['search_button'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#weight' => 3,
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body']['buttons']['add_button'] = array(
        '#type' => 'submit',
        '#value' => 'Tạo lịch phát sóng',
        '#weight' => 2,
        '#submit' => array('create_new_broadcast_submit'),
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-success')
        ),
    );
    $message = '<h4>Tổng số lịch phát sóng là : '.$total_result.'</h4>';

    $form['list_broadcast']['body']['search_summary'] = array(
        '#type' => 'item',
        '#weight' => 2,
        '#markup' => '<div class="callout callout-info">'.$message.'</div>',
        '#prefix' => '<div class="col-md-12" style="margin-top : 30px;">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['result'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có lịch phát sóng nào',
        '#attributes' => array('class'=>array('table','table-striped')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );

    return $form;
}

function broadcast_list_form_submit($form,&$form_state){
    global $base_url;
    $values = $form_state['values'];

    $company_id = isset($values['broadcast_company']) ? $values['broadcast_company'] : 0;
    $region_id = isset($values['broadcast_region']) ? $values['broadcast_region'] : 0;
    $province_id = isset($values['broadcast_province']) ? $values['broadcast_province'] : 0;
    $store_id = isset($values['broadcast_store']) ? $values['broadcast_store'] : 0;
    $status = isset($values['status']) ? $values['status'] : 0;
    $start_date = isset($values['start_date']) ? $values['start_date'] : 0;
    $end_date = isset($values['end_date']) ? $values['end_date'] : 0;

    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
        'store_id' => $store_id,
        'status' => $status,
        'page' => isset($values['page']) ? $values['page'] : 0,
        'start_date' => $start_date,
        'end_date' => $end_date
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/broadcast/list?'.$query_string;
}

function create_new_broadcast_submit($form,&$form_state){
    global $base_url;
    $values = $form_state['values'];

    $company_id = isset($values['broadcast_company']) ? $values['broadcast_company'] : 0;
    $region_id = isset($values['broadcast_region']) ? $values['broadcast_region'] : 0;
    $province_id = isset($values['broadcast_province']) ? $values['broadcast_province'] : 0;
    $store_id = isset($values['broadcast_store']) ? $values['broadcast_store'] : 0;

    $start_date = isset($values['start_date']) ? $values['start_date'] : 0;
    $end_date = isset($values['end_date']) ? $values['end_date'] : 0;

    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
        'store_id' => $store_id,
        'page' => isset($values['page']) ? $values['page'] : 0,
        'start_date' => $start_date,
        'end_date' => $end_date
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/broadcast/create?'.$query_string;
}


function list_broadcast($company_id=0,$region_id=0,$province_id=0,$store_id=0,$status=0,$start_date=0,$end_date=0,$header=array(),$number_per_page=50,$page=0){
    global $base_url,$user;
    $rows = array();
    $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort')->distinct()->limit($number_per_page);
    $query->fields('n',array('nid','uid','created'));
    $query->condition('n.type','broadcast_playlist');
    if(in_array('marketing staff',$user->roles)){
        $query->condition('n.uid',$user->uid);
    }


    $query->leftJoin('field_data_field_start_time_playlist','stp','stp.entity_id=n.nid');
    $query->leftJoin('field_data_field_end_time_playlist','etp','etp.entity_id=n.nid');


    if($company_id){
        $query->leftJoin('field_data_field_company_id_playlist','cip','cip.entity_id=n.nid');
        $query->condition('field_company_id_playlist_tid',$company_id);
    }
    if($region_id){
        $query->leftJoin('field_data_field_region_id_playlist','rip','rip.entity_id=n.nid');
        $query->condition('field_region_id_playlist_tid',$region_id);
    }
    if($province_id){
        $query->leftJoin('field_data_field_province_id_playlist','pip','pip.entity_id=n.nid');
        $query->condition('field_province_id_playlist_tid',$province_id);
    }
    if($store_id){
        $query->leftJoin('field_data_field_store_id_playlist','sip','sip.entity_id=n.nid');
        $query->condition('field_store_id_playlist_tid',$store_id);
    }
    if($status){
        $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
        $query->condition('field_approval_playlist_value',$status);
    }
    if($start_date){
        $query->condition('field_start_time_playlist_value',strtotime($start_date),'>=');
    }
    if($end_date){
        $query->condition('field_end_time_playlist_value',strtotime($end_date),'<=');
    }

    $query->orderBy('field_end_time_playlist_value','DESC');
    $query->orderBy('field_start_time_playlist_value','DESC');
    $query->condition('n.status',1,'=');
    $total_result = $query->countQuery()->execute()->fetchField();
    $result = $query->execute();
    $count = $page ? (($page*$number_per_page)+1) : 1;
    if($result->rowCount()){
        foreach($result as $row){
            $broadcast = node_load($row->nid);

            //Load Playlist by broadcast_id
            $query = db_select('node','n');
            $query->fields('n',array('nid'));
            $query->fields('bps',array('field_broadcast_playlist_start_value'));
            $query->condition('n.type','broadcast_playlist_time');
            $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
            $query->condition('field_broadcast_playlist_time_id_nid',$row->nid);
            $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');
            $query->orderBy('field_broadcast_playlist_start_value','DESC');

            $broadcast_time_result = $query->execute();
            if($broadcast_time_result->rowCount()){
                $number_playlist = $broadcast_time_result->rowCount();
            }else{
                $number_playlist = 0;
            }

            $term_company = taxonomy_term_load($broadcast->field_company_id_playlist['und'][0]['tid']);
            $company_name = $term_company->name;
            if(isset($broadcast->field_region_id_playlist['und'][0]['tid']) && $broadcast->field_region_id_playlist['und'][0]['tid']){
                $term_region = taxonomy_term_load($broadcast->field_region_id_playlist['und'][0]['tid']);
                $region_name = $term_region->name;
            }else{
                $region_name = '';
            }
            if(isset($broadcast->field_province_id_playlist['und'][0]['tid']) && $broadcast->field_province_id_playlist['und'][0]['tid']){
                $term_province = taxonomy_term_load($broadcast->field_province_id_playlist['und'][0]['tid']);
                $province_name = $term_province->name;
            }else{
                $province_name = '';
            }
            $store_name = '';
            if(isset($broadcast->field_store_id_playlist['und']) && $broadcast->field_store_id_playlist['und']){
                $store_ids = $broadcast->field_store_id_playlist['und'];
                foreach($store_ids as $tid){
                    $term_store = taxonomy_term_load($tid['tid']);
                    $store_name .= isset($term_store->name) ? $term_store->name : ''.'<br/>';
                }

            }else{
                $store_name = '';
            }
            $current_date = strtotime(date('Y-m-d'));

            $broadcast_name = '';

            if($broadcast->field_end_time_playlist['und'][0]['value'] < $current_date) {
                $broadcast_name = '<span class="label label-danger"><a href="' . $base_url . '/broadcast/view/playlist/' . $broadcast->nid . '" style="color:#FFF;padding:10px;">' . $broadcast->title . '</a></span>';
                $day_left = '<span class="label label-danger">0</span>';
            }elseif(($broadcast->field_start_time_playlist['und'][0]['value'] <= $current_date) && ($broadcast->field_end_time_playlist['und'][0]['value'] >= $current_date)){

                //Days to go
                $now = time(); // or your date as well
                $end_date = $broadcast->field_end_time_playlist['und'][0]['value'];
                $datediff = $end_date - $now;
                $day_to_go = floor($datediff / (60 * 60 * 24)) + 1;

                if($day_to_go < 3){
                    $broadcast_name = '<span class="label label-warning"><a href="' . $base_url . '/broadcast/view/playlist/' . $broadcast->nid . '" style="color:#FFF;padding:10px;">' . $broadcast->title . '</a></span>';
                    $day_left = '<span class="label label-warning">'.$day_to_go.'</span>';
                }else{
                    $day_left = '<span class="label label-success">'.$day_to_go.'</span>';
                    $broadcast_name = '<span class="label label-success"><a href="' . $base_url . '/broadcast/view/playlist/' . $broadcast->nid . '" style="color:#FFF;padding:10px;">' . $broadcast->title . '</a></span>';
                }
            }

            //Lấy trạng thái lịch phát sóng
            $label_status_broadcast = get_label_broadcast_status($broadcast);
            //Reviewer
            $reviewer = user_load($row->uid);
            $rows[] = array(
                $count,
                '<div>'.date('d/m/Y',$broadcast->field_start_time_playlist['und'][0]['value']).'</div><div>'.date('d/m/Y',$broadcast->field_end_time_playlist['und'][0]['value']).'</div>',
                $broadcast_name,
                $day_left,
                $label_status_broadcast,
                $reviewer->name,
                date('d/m/Y H:i:s',$row->created),
                $company_name,
                $region_name,
                $province_name,
                $store_name,
                '<div class="btn-group">
                    <button type="button" class="btn btn-success">Thao tác</button>
                    <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                        <span class="caret"></span>
                        <span class="sr-only">Toggle Dropdown</span>
                    </button>
                    <ul class="dropdown-menu" role="menu">
                        <li><a href="'.$base_url.'/broadcast/edit/'.$broadcast->nid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                        <li><a href="'.$base_url.'/broadcast/delete/'.$broadcast->nid.'" ><i class="fa fa-trash"></i> Xóa</a></li>
                    </ul>
                </div>'
            );
            $count++;
        }
    }
    $return['rows'] = $rows;
    $return['total_result'] = $total_result;
    return $return;
}


function broadcast_create_form($form,&$form_state){
    $form = array();
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $store_id = isset($_GET['store_id']) ? $_GET['store_id'] : 0;

    $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : 0;
    $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : 0;

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
    $form['list_broadcast']['header']['header_title'] = array(
        '#type' => 'item',
        '#weight' => 2,
        '#markup' => '<h3>Bước 1 : Thông tin lịch phát sóng</h3>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['info']['col_left'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['info']['col_right'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['info']['col_left']['broadcast_name'] = array(
        '#type' => 'textfield',
        '#title' => 'Tên lịch phát sóng',
        '#required' => TRUE,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['info']['col_left']['broadcast_description'] = array(
        '#type' => 'textarea',
        '#title' => 'Mô tả',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['info']['col_right']['start_date'] = array(
        '#type' => 'date_popup',
        //'#timepicker' => 'timepicker',
        '#title' => 'Từ ngày',
        '#date_label_position' => 'none',
        '#default_value' => $start_date,
        '#date_format' => 'd/m/Y',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>',
        '#size' => 70

    );

    $form['list_broadcast']['body']['info']['col_right']['end_date'] = array(
        '#type' => 'date_popup',
        //'#timepicker' => 'timepicker',
        '#title' => 'Tới ngày',
        '#date_label_position' => 'none',
        '#default_value' => $end_date,
        '#date_format' => 'd/m/Y',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
        '#size' => 70
    );

    $form['list_broadcast']['body']['broadcast_area'] = array(
        '#type' => 'fieldset',
        '#weight' => 2,
        '#title' => 'Phát sóng cho : ',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $company_options = load_voca_info('company');
    $form['list_broadcast']['body']['broadcast_area']['broadcast_company'] = array(
        '#type' => 'select',
        '#title' => 'Công ty',
        '#options' => $company_options,
        '#default_value' => $company_id,
        '#prefix' => '<div class="col-md-4">',
        '#suffix' => '</div>',
        '#required' => TRUE,
        '#ajax' => array(
            'callback' => 'region_select_callback',
            'wrapper' => 'region-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
        '#validated' => TRUE,
    );
    if($company_id){
        $region_options = load_voca_info('region',array('company_id'=>$company_id));
    }else{
        $region_options = array(0=>t('Chọn vùng miền'));
    }
    $form['list_broadcast']['body']['broadcast_area']['broadcast_region'] = array(
        '#type' => 'select',
        '#title' => 'Vùng miền',
        '#options' => $region_options,
        '#default_value' => $region_id,
        '#prefix' => '<div id="region-wrapper" class="col-md-4">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'province_select_callback',
            'wrapper' => 'province-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );
    if($region_id){
        $province_options = load_voca_info('province',array('company_id'=>$company_id,'region_id'=>$region_id));
    }else{
        $province_options = array(0=>('Chọn tỉnh/thành'));
    }
    $form['list_broadcast']['body']['broadcast_area']['broadcast_province'] = array(
        '#type' => 'select',
        '#title' => 'Tỉnh / Thành',
        '#options' => $province_options,
        '#default_value' => $province_id,
        '#prefix' => '<div id="province-wrapper" class="col-md-4">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'store_select_callback',
            'wrapper' => 'store-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );

    if($province_id){
        $store_options = load_store_info('store',array('company_id'=>$company_id,'region_id'=>$region_id,'province_id'=>$province_id));
    }else{
        $store_options = 'Vui lòng chọn cửa hàng';
    }

    $form['list_broadcast']['body']['broadcast_area']['broadcast_store'] = array(
        '#type' => 'item',
        '#title' =>'Cửa hàng',
        '#markup' => $store_options,
        '#prefix' => '<div id="store-wrapper" class="col-md-12">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );

    $form['list_broadcast']['footer'] = array(
        '#type' => 'container',
        '#weight' => 12,
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['footer']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Lưu & Thêm Playlist',
        '#attributes' => array(
            'class' => array('btn','btn-info')
        ),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>'
    );


    return $form;
}

function region_select_callback($form, &$form_state){
    $company_id = $form_state['values']['broadcast_company'];
    $options = load_voca_info('region',array('company_id'=>$company_id));
    $form['list_broadcast']['body']['broadcast_area']['broadcast_region']['#options'] = array(0=>t('Chọn vùng miền')) + $options;
    return $form['list_broadcast']['body']['broadcast_area']['broadcast_region'];

}
function province_select_callback($form, &$form_state){
    $region_id = $form_state['values']['broadcast_region'];
    $options = load_voca_info('province',array('region_id'=>$region_id));
    $form['list_broadcast']['body']['broadcast_area']['broadcast_province']['#options'] = array(0=>t('Chọn tỉnh/thành')) + $options;
    return $form['list_broadcast']['body']['broadcast_area']['broadcast_province'];
}
function store_select_callback($form, &$form_state){
    $province_id = $form_state['values']['broadcast_province'];
    $options = load_store_info('store',array('province_id'=>$province_id));
    $form['list_broadcast']['body']['broadcast_area']['broadcast_store']['#markup'] = $options;
    return $form['list_broadcast']['body']['broadcast_area']['broadcast_store'];
}

function load_voca_info($voca = 'company',$info=array('company_id' => 0,'region_id'=>0,'province_id'=>0,'store_id'=>0)){
    $options = array();
    global $user;
    $user_info = user_load($user->uid);

    switch ($voca){
        case 'company':
            $options[0] = 'Chọn công ty';
            $vocabulary = taxonomy_vocabulary_machine_name_load($voca);
            $terms = entity_load('taxonomy_term', FALSE, array('vid' => $vocabulary->vid));

            if(in_array('marketing staff',$user->roles)){
                $role_company_tid = isset($user_info->field_user_company_id['und'][0]['tid']) ? $user_info->field_user_company_id['und'][0]['tid'] : 0;

                if($role_company_tid){
                    $role_term = taxonomy_term_load($role_company_tid);
                    $options[$role_term->tid] = $role_term->name;
                }
            }else{
                foreach($terms as $term){
                    $options[$term->tid] = $term->name;
                }
            }


            break;
        case 'region':
            $options[0] = 'Chọn vùng miền';
            if(in_array('marketing staff',$user->roles)){
                $role_region_tids = isset($user_info->field_user_region_id['und']) ? $user_info->field_user_region_id['und'] : 0;
                if($role_region_tids) {
                    foreach ($role_region_tids as $region_tid) {
                        $tids[] = $region_tid['tid'];
                    }
                    $terms = taxonomy_term_load_multiple($tids);
                    foreach ($terms as $term) {
                        $options[$term->tid] = $term->name;
                    }
                }
            }else{
                $tids = db_select('field_data_field_region_company_id','rbycom')
                    ->fields('rbycom',array('entity_id'))
                    ->condition('field_region_company_id_tid',$info['company_id'],'=')
                    ->execute()
                    ->fetchCol();
                $terms = taxonomy_term_load_multiple($tids);
                foreach($terms as $term){
                    $options[$term->tid] = $term->name;
                }
            }

            break;
        case 'province':
            $options[0] = 'Chọn tỉnh/thành';
            if(in_array('marketing staff',$user->roles)) {
                $role_province_tids = isset($user_info->field_user_province_id['und']) ? $user_info->field_user_province_id['und'] : 0;
                if($role_province_tids){
                    foreach($role_province_tids as $role_province_tid){
                        $tids[] = $role_province_tid['tid'];
                    }
                    $terms = taxonomy_term_load_multiple($tids);
                    foreach($terms as $term){
                        $options[$term->tid] = $term->name;
                    }
                }

            }else{
                $tids = db_select('field_data_field_region_provice_id','pbyrg')
                    ->fields('pbyrg',array('entity_id'))
                    ->condition('field_region_provice_id_tid',$info['region_id'],'=')
                    ->execute()
                    ->fetchCol();
                $terms = taxonomy_term_load_multiple($tids);
                foreach($terms as $term){
                    $options[$term->tid] = $term->name;
                }
            }

            break;
        case 'store':
            $options[0] = 'Chọn cửa hàng';
            $tids = db_select('field_data_field_province_store_id','stbypr')
                ->fields('stbypr',array('entity_id'))
                ->condition('field_province_store_id_tid',$info['province_id'],'=')
                ->execute()
                ->fetchCol();
            $terms = taxonomy_term_load_multiple($tids);
            foreach($terms as $term){
                $options[$term->tid] = $term->name;
            }
            break;

    }
    return $options;
}

function load_store_info($voca = 'company',$info=array('company_id' => 0,'region_id'=>0,'province_id'=>0,'store_id'=>0),$store_tids = array()){
    $vocabulary = taxonomy_vocabulary_machine_name_load($voca);
    $query = db_select('taxonomy_term_data','ttd');
    $query->fields('ttd',array('tid','name'));
    $query->condition('ttd.vid',$vocabulary->vid);
    $query->leftJoin('field_data_field_province_store_id','psi','psi.entity_id=ttd.tid');
    $query->condition('field_province_store_id_tid',$info['province_id']);
    $query->orderBy('name','asc');
    $terms = $query->execute();
    $stores = '';
    foreach($terms as $term) {
        if(in_array($term->tid,$store_tids)){
            $stores .= '<div class="col-md-3"><input type="checkbox" name="store[]" value="'.$term->tid.'" checked="checked"> '.$term->name.'</div>';
        }else{
            $stores .= '<div class="col-md-3"><input type="checkbox" name="store[]" value="'.$term->tid.'"> '.$term->name.'</div>';
        }

    }

    return $stores;
}

function broadcast_create_form_submit($form,&$form_state){
    global $user,$base_url;
    $values = $form_state['values'];
    $input = $form_state['input'];

    if($values['broadcast_company']){
        $company_tid = $values['broadcast_company'];
    }else{
        $company_tid = 0;
    }
    if($values['broadcast_region']){
        $region_tid = $values['broadcast_region'];
    }else{
        $region_tid = 0;
    }
    if($values['broadcast_province']){
        $province_tid = $values['broadcast_province'];
    }else{
        $province_tid = 0;
    }

    if($input['store']){
        $stores = $input['store'];

        foreach($stores as $key=>$tid){
            if($tid){
                $store_tid[] = array('tid' => $tid);
            }
        }
    }else{
        $store_tid = 0;
    }

    if($store_tid && $province_tid && $region_tid && $company_tid){
        $level_access = 4;
    }else if(!$store_tid && $province_tid && $region_tid && $company_tid){
        $level_access = 3;
    }else if(!$store_tid && !$province_tid && $region_tid && $company_tid){
        $level_access = 2;
    }else if(!$store_tid && !$province_tid && !$region_tid && $company_tid){
        $level_access = 1;
    }

    $broadcast = new stdClass();
    $broadcast->type = 'broadcast_playlist';
    $broadcast->title = $values['broadcast_name'];
    $broadcast->body['und'][0]['value'] = $values['broadcast_description'];
    $broadcast->field_start_time_playlist['und'][0]['value'] = strtotime($values['start_date']);
    $broadcast->field_end_time_playlist ['und'][0]['value'] = strtotime($values['end_date']);
    if($company_tid){
        $broadcast->field_company_id_playlist['und'][0]['tid'] = $company_tid;
    }
    if($region_tid) {
        $broadcast->field_region_id_playlist['und'][0]['tid'] = $region_tid;
    }
    if($province_tid){
        $broadcast->field_province_id_playlist['und'][0]['tid'] = $province_tid;
    }
    if($store_tid){
        $broadcast->field_store_id_playlist['und'] = $store_tid;
    }
    $broadcast->field_level_access['und'][0]['value'] = $level_access;
    $broadcast->language = LANGUAGE_NONE;
    $broadcast->uid = $user->uid;

    node_save($broadcast);
    if($broadcast->nid){
        $form_state['redirect'] = 'broadcast/add/playlist/'.$broadcast->nid;
    }
}

function broadcast_edit_playlist_form($form,&$form_state,$broadcast_id){
    global $base_url;
    $broadcast = node_load($broadcast_id);
    $stores = isset($broadcast->field_store_id_playlist['und']) ? $broadcast->field_store_id_playlist['und'] : 0;
    $store_tids = array();
    if($stores){
        foreach($stores as $store_tid){
            $store_tids[] = $store_tid['tid'];
        }
    }
    $is_used = 0;
    if($broadcast->field_approval_playlist['und'][0]['value']==2){
        $is_used = 1;
    }
    if($is_used){
        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),

        );

        $header_title = 'Bạn không thể điều chỉnh Lịch phát sóng đã được phê duyệt!';

        $form['list_broadcast']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-12">'.$header_title.'</h3>',
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
            '#markup' => '<b>Lịch phát sóng : </b>' .$broadcast->title,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );


        $form['list_broadcast']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['footer']['buttons'] = array(
            '#type' => 'container',
            '#weight' => 3,
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );

        $form['list_broadcast']['footer']['buttons']['back_broadcast'] = array(
            '#type' => 'item',
            '#markup' => '<a href="'.$base_url.'/broadcast/list" class="btn btn-info"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="pull-right">',
            '#suffix' => '</div>'
        );
    }else {
        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box', 'box-success')
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
            '#weight' => 2,
            '#markup' => '<h3 class="col-md-10">Thông tin lịch phát sóng</h3> <a href="'.$base_url.'/broadcast/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>',


        );
        $form['list_broadcast']['body'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="box-body with-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['info'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['info']['col_left'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['info']['col_right'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['info']['col_left']['broadcast_name'] = array(
            '#type' => 'textfield',
            '#title' => 'Tên lịch phát sóng',
            '#value' => $broadcast->title,
            '#required' => TRUE,
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['info']['col_left']['broadcast_description'] = array(
            '#type' => 'textarea',
            '#title' => 'Mô tả',
            '#value' => isset($broadcast->body['und'][0]['value']) ? $broadcast->body['und'][0]['value'] : '',
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['info']['col_right']['start_date'] = array(
            '#type' => 'date_popup',
            //'#timepicker' => 'timepicker',
            '#title' => 'Từ ngày',
            '#default_value' => date('Y-m-d', $broadcast->field_start_time_playlist['und'][0]['value']),
            '#date_label_position' => 'none',
            '#date_format' => 'd/m/Y',
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>',
            '#size' => 70
        );
        $form['list_broadcast']['body']['info']['col_right']['end_date'] = array(
            '#type' => 'date_popup',
            //'#timepicker' => 'timepicker',
            '#title' => 'Tới ngày',
            '#default_value' => date('Y-m-d', $broadcast->field_end_time_playlist['und'][0]['value']),
            '#date_label_position' => 'none',
            '#date_format' => 'd/m/Y',
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>',
            '#size' => 70
        );


        $form['list_broadcast']['body']['broadcast_area'] = array(
            '#type' => 'fieldset',
            '#weight' => 2,
            '#title' => 'Phát sóng cho : ',
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        $company_options = load_voca_info('company');
        $form['list_broadcast']['body']['broadcast_area']['broadcast_company'] = array(
            '#type' => 'select',
            '#title' => 'Công ty',
            '#options' => $company_options,
            '#prefix' => '<div class="col-md-4">',
            '#suffix' => '</div>',
            '#default_value' => isset($broadcast->field_company_id_playlist['und'][0]['tid']) ? $broadcast->field_company_id_playlist['und'][0]['tid'] : '',
            '#required' => TRUE,
            '#ajax' => array(
                'callback' => 'region_select_callback',
                'wrapper' => 'region-wrapper',
                'method' => 'replace',
                'effect' => 'fade',
            ),
            '#validated' => TRUE,
        );

        if (isset($broadcast->field_company_id_playlist['und'][0]['tid']) && $broadcast->field_company_id_playlist['und'][0]['tid']) {
            $region_options = load_voca_info('region', array('company_id' => $broadcast->field_company_id_playlist['und'][0]['tid']));
        } else {
            $region_options = array(0 => t('Chọn vùng miền'));
        }
        $form['list_broadcast']['body']['broadcast_area']['broadcast_region'] = array(
            '#type' => 'select',
            '#title' => 'Vùng miền',
            '#options' => $region_options,
            '#prefix' => '<div id="region-wrapper" class="col-md-4">',
            '#suffix' => '</div>',
            '#default_value' => isset($broadcast->field_region_id_playlist['und'][0]['tid']) ? $broadcast->field_region_id_playlist['und'][0]['tid'] : '',
            '#validated' => TRUE,
            '#ajax' => array(
                'callback' => 'province_select_callback',
                'wrapper' => 'province-wrapper',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );
        if (isset($broadcast->field_region_id_playlist['und'][0]['tid']) && $broadcast->field_region_id_playlist['und'][0]['tid']) {
            $province_options = load_voca_info('province', array('region_id' => $broadcast->field_region_id_playlist['und'][0]['tid']));
        } else {
            $province_options = array(0 => ('Chọn tỉnh/thành'));
        }

        $form['list_broadcast']['body']['broadcast_area']['broadcast_province'] = array(
            '#type' => 'select',
            '#title' => 'Tỉnh / Thành',
            '#options' => $province_options,
            '#prefix' => '<div id="province-wrapper" class="col-md-4">',
            '#suffix' => '</div>',
            '#default_value' => isset($broadcast->field_province_id_playlist['und'][0]['tid']) ? $broadcast->field_province_id_playlist['und'][0]['tid'] : '',
            '#validated' => TRUE,
            '#ajax' => array(
                'callback' => 'store_select_callback',
                'wrapper' => 'store-wrapper',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );
        $company_id = isset($broadcast->field_company_id_playlist['und'][0]['tid']) ? $broadcast->field_company_id_playlist['und'][0]['tid'] : 0;
        $region_id = isset($broadcast->field_region_id_playlist['und'][0]['tid']) ? $broadcast->field_region_id_playlist['und'][0]['tid'] : 0;
        $province_id = isset($broadcast->field_province_id_playlist['und'][0]['tid']) ? $broadcast->field_province_id_playlist['und'][0]['tid'] : 0;
        if ($province_id) {
            $store_options = load_store_info('store', array('company_id' => $company_id, 'region_id' => $region_id, 'province_id' => $province_id), $store_tids);
        } else {
            $store_options = 'Vui lòng chọn cửa hàng';
        }

        $form['list_broadcast']['body']['broadcast_area']['broadcast_store'] = array(
            '#type' => 'item',
            '#title' => 'Cửa hàng',
            '#markup' => $store_options,
            '#prefix' => '<div id="store-wrapper" class="col-md-12">',
            '#suffix' => '</div>',
            '#validated' => TRUE,
        );

        $rows = array();
        //Load Playlist by broadcast_id
        $query = db_select('node', 'n');
        $query->fields('n', array('nid'));
        $query->fields('bps', array('field_broadcast_playlist_start_value'));
        $query->condition('n.type', 'broadcast_playlist_time');
        $query->leftJoin('field_data_field_broadcast_playlist_time_id', 'bpt', 'bpt.entity_id=n.nid');
        $query->condition('field_broadcast_playlist_time_id_nid', $broadcast_id);
        $query->leftJoin('field_data_field_broadcast_playlist_start', 'bps', 'bps.entity_id=n.nid');
        $query->orderBy('field_broadcast_playlist_start_value', 'ASC');

        $broadcast_time_result = $query->execute();
        if ($broadcast_time_result->rowCount()) {
            foreach ($broadcast_time_result as $broadcast_time) {
                $broadcast_time = node_load($broadcast_time->nid);
                $playlist = node_load($broadcast_time->field_playlist_broadcast_time_id['und'][0]['nid']);
                $order = 1;
                $number_slides = isset($playlist->field_slides_playlist['und']) ? count($playlist->field_slides_playlist['und']) : 0;
                //Tính thời gian của playlist
                $slide_ids = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : 0;
                $total_time = 0;
                if ($slide_ids) {
                    foreach ($slide_ids as $slide_id) {
                        foreach ($slide_id as $key => $nid) {
                            if ($nid) {
                                $slide = node_load($nid);
                                $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;

                            }
                        }
                    }
                }

                $rows[] = array(
                    $playlist->title,
                    $number_slides,
                    gmdate('H:i:s', $total_time),
                    array(
                        'data' => array(
                            '#type' => 'textfield',
                            '#title' => 'Thời gian bắt đầu',
                            '#title_display' => 'invisible',
                            '#value' => isset($broadcast_time->field_broadcast_playlist_start['und'][0]['value']) ? date('H:i:s', $broadcast_time->field_broadcast_playlist_start['und'][0]['value']) : '',
                            '#name' => 'start_time[' . $broadcast_time->nid . ']',
                            '#size' => 5,
                            '#required' => TRUE
                        ),
                    ),
                    array(
                        'data' => array(
                            '#type' => 'textfield',
                            '#title' => 'Thời gian bắt đầu',
                            '#title_display' => 'invisible',
                            '#value' => isset($broadcast_time->field_broadcast_playlist_end['und'][0]['value']) ? date('H:i:s', $broadcast_time->field_broadcast_playlist_end['und'][0]['value']) : '',
                            '#name' => 'end_time[' . $broadcast_time->nid . ']',
                            '#size' => 5,
                            '#required' => TRUE
                        ),
                    ),
                );
            }
        }

        $header = array(
            array('data' => 'Tên Playlist'),
            array('data' => 'Số Files'),
            array('data' => 'Thời lượng'),
            array('data' => 'Thời gian bắt đầu'),
            array('data' => 'Thời gian kết thúc'),
        );


        $form['playlists_info'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box', 'box-info')
            ),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['playlists_info']['header'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['playlists_info']['header']['header_title'] = array(
            '#type' => 'item',
            '#weight' => 2,
            '#markup' => '<h3>Danh sách playlist</h3>',
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        $form['playlists_info']['body'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="box-body with-border">',
            '#suffix' => '</div>'
        );
        $form['playlists_info']['body']['playlists'] = array(
            '#weight' => 3,
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
            '#empty' => 'Chưa có Nội dung nào',
            '#attributes' => array('class' => 'table table-striped'),
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );


        $form['list_broadcast']['broadcast_id'] = array(
            '#type' => 'hidden',
            '#value' => $broadcast_id
        );

        $form['playlists_info']['footer'] = array(
            '#type' => 'container',
            '#weight' => 12,
            '#prefix' => '<div class="box-footer with-border">',
            '#suffix' => '</div>'
        );

        $form['playlists_info']['footer']['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Lưu lịch phát sóng',
            '#attributes' => array(
                'class' => array('btn', 'btn-info')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>',

        );
    }

    return $form;
}
function broadcast_edit_playlist_form_validate($form,&$form_state){
    $values = $form_state['values'];
    $input = $form_state['input'];
    $start_date = strtotime($values['start_date']);
    $end_date = strtotime($values['end_date']);
    if($start_date > $end_date){
        drupal_set_message('Ngày kết thúc lớn hơn ngày bắt đầu. Vui lòng chọn lại','error');
    }
}

function broadcast_edit_playlist_form_submit($form, &$form_state){

    global $base_url;
    $values = $form_state['values'];
    $input = $form_state['input'];
    $broadcast_id = $values['broadcast_id'];
    $broadcast = node_load($broadcast_id);

    $stores = isset($broadcast->field_store_id_playlist['und']) ? $broadcast->field_store_id_playlist['und'] : array();
    $store_tids = array();
    if($stores){
        foreach($stores as $store_tid){
            $store_tids[] = $store_tid['tid'];
        }
    }

    if($values['broadcast_company']){
        $company_tid = $values['broadcast_company'];
    }else{
        $company_tid = 0;
    }
    if($values['broadcast_region']){
        $region_tid = $values['broadcast_region'];
    }else{
        $region_tid = 0;
    }
    if($values['broadcast_province']){
        $province_tid = $values['broadcast_province'];
    }else{
        $province_tid = 0;
    }
    $store_tid = array();
    if(isset($input['store'])&&$input['store']){
        $stores_input = $input['store'];

        foreach($stores_input as $key=>$tid){
            if($tid){
                $store_tid[] = array('tid' => $tid);
            }
        }

    }

    if($store_tid && $province_tid && $region_tid && $company_tid){
        $level_access = 4;
    }else if(!$store_tid && $province_tid && $region_tid && $company_tid){
        $level_access = 3;
    }else if(!$store_tid && !$province_tid && $region_tid && $company_tid){
        $level_access = 2;
    }else if(!$store_tid && !$province_tid && !$region_tid && $company_tid){
        $level_access = 1;
    }
    $update_broadcast = node_load($broadcast_id);
    $update_broadcast->title = isset($input['broadcast_name']) ? $input['broadcast_name'] : '';
    $update_broadcast->body['und'][0]['value'] = isset($input['broadcast_description']) ? $input['broadcast_description'] : '';
    $update_broadcast->field_start_time_playlist['und'][0]['value'] = strtotime($values['start_date']);
    $update_broadcast->field_end_time_playlist['und'][0]['value'] = strtotime($values['end_date']);
    $update_broadcast->field_company_id_playlist['und'][0]['tid'] = $values['broadcast_company'];
    $update_broadcast->field_region_id_playlist['und'][0]['tid'] = $values['broadcast_region'];
    $update_broadcast->field_province_id_playlist['und'][0]['tid'] = $values['broadcast_province'];
    $update_broadcast->field_store_id_playlist['und'] = $store_tid;
    $update_broadcast->field_level_access['und'][0]['value'] = $level_access;

    node_save($update_broadcast);
    //Update time play the playlist for this broadcast

    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->condition('n.type','broadcast_playlist_time');
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
    $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);

    $broadcast_time_result = $query->execute();
    if($broadcast_time_result->rowCount()) {
        foreach ($broadcast_time_result as $broadcast_time_id) {
            $broadcast_time = node_load($broadcast_time_id->nid);
            $broadcast_time->field_broadcast_playlist_start['und'][0]['value'] = strtotime($values['start_date']. ' '.$input['start_time'][$broadcast_time_id->nid]);
            $broadcast_time->field_broadcast_playlist_end['und'][0]['value'] = strtotime($values['end_date']. ' '.$input['end_time'][$broadcast_time_id->nid]);
            $broadcast_time->status = 1;
            node_save($broadcast_time);

        }
    }
    $form_state['redirect'] = $base_url.'/broadcast/view/playlist/'.$values['broadcast_id'];
}


function broadcast_add_playlist_form($form,&$form_state,$broadcast_id){
    drupal_add_js(drupal_get_path('module','adtrue').'/js/adtrue.js');
    global $user, $base_url;

    $broadcast = node_load($broadcast_id);
    $form['broadcast_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>'.$broadcast->title.'</h2>',
        '#prefix' => '<div class="col-md-10">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['header']['back_playlist'] = array(
        '#type' => 'item',
        '#markup' => '<a href="'.$base_url.'/broadcast/view/playlist/'.$broadcast_id.'" class="btn btn-info pull-right" style="margin-top:25px;"><i class="fa fa-reply"></i> Quay lại</a>',
        '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['info'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->body['und'][0]['value']) ? '<b>Mô tả : </b>'.$broadcast->body['und'][0]['value'] : '<b>Mô tả : </b>'.'Chưa có Mô tả cho lịch phát sóng. Vui lòng cập nhật!',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['start_time'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_start_time_playlist['und'][0]['value']) ? '<b>Ngày phát sóng : </b>'.date('d-m-Y',$broadcast->field_start_time_playlist['und'][0]['value']) : '',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $header =  array(
        'name'=>'Tên Playlist',
        'number'=>'Số Files',
        'duration' => 'Thời lượng',
        'category'=>'Danh mục',
        'action' => 'Thao tác'
    );


    if(isset($form_state['storage']['rows']) && $form_state['storage']['rows']){
        $rows = $form_state['storage']['rows'];
    }else {
        $rows = load_playlist_search($broadcast_id);

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

    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['playlists'] = array(
        '#weight' => 3,
        '#type' => 'tableselect',
        '#header' => $header,
        '#options' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>array('table','table-striped')),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['footer'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['broadcast_id'] = array(
        '#type' => 'hidden',
        '#value' => $broadcast_id
    );
    $form['list_broadcast']['footer']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Lưu nội dung',
        '#attributes' => array(
            'class' => array('btn','btn-info')
        ),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',
        '#submit' => array('broadcast_add_playlist_submit')
    );
    return $form;
}

function broadcast_add_playlist_form_submit($form,&$form_state){
    $rows = array();
    $broadcast_id =  isset($form_state['values']['broadcast_id']) ? $form_state['values']['broadcast_id'] : '';
    $playlist_category =  isset($form_state['values']['playlist_category']) ? $form_state['values']['playlist_category'] : '';
    $playlist_title = isset($form_state['values']['playlist_title']) ? $form_state['values']['playlist_title'] : '';
    $rows = load_playlist_search($broadcast_id,$playlist_category,$playlist_title);
    $form_state['storage']['rows'] = $rows;
    $form_state['rebuild'] = TRUE;
}

function broadcast_add_playlist_submit($form,&$form_state){
    global  $user,$base_url;
    $values = $form_state['values'];
    $playlists = $values['playlists'];

    $broadcast_id = $values['broadcast_id'];
    $broadcast = node_load($broadcast_id);
    $order = 1;
    $total_files = 0;
    $total_playlist = 0;
    foreach($playlists as $key=>$value){
        if($value){
            $playlist = node_load($value);
            $number_files_playlist = isset($playlist->field_playlist_total_files['und'][0]['value']) ? $playlist->field_playlist_total_files['und'][0]['value'] : 0;
            $total_files += $number_files_playlist;
            //Tạo Broadcast Playlist Time
            $broadcast_playlist_time = new stdClass();
            $broadcast_playlist_time->type = 'broadcast_playlist_time';
            $broadcast_playlist_time->title = $broadcast->title;
            $broadcast_playlist_time->field_broadcast_playlist_time_id['und'][0]['nid'] = $broadcast_id;
            $broadcast_playlist_time->field_playlist_broadcast_time_id['und'][0]['nid'] = $value;
            $broadcast_playlist_time->field_broadcast_playlist_order['und'][0]['value'] = $order;
            $broadcast_playlist_time->language = LANGUAGE_NONE;
            $broadcast_playlist_time->uid = $user->uid;
            $broadcast_playlist_time->status = 0;
            node_save($broadcast_playlist_time);
            $order++;
            $total_playlist++;
        }
    }
    $broadcast->field_broadcast_total_files['und'][0]['value'] = $total_files;
    $broadcast->field_broadcast_total_playlists['und'][0]['value'] = $total_playlist;

    node_save($broadcast);
    $form_state['redirect'] = 'broadcast/set_time/playlist/'.$broadcast_id;
}

function broadcast_set_time_playlist_form($form,&$form_state,$broadcast_id){
    global $base_url;
    $broadcast = node_load($broadcast_id);

    $form['broadcast_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
    );
    $form['broadcast_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>'.$broadcast->title.'</h2>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    /*
    $form['broadcast_info']['header']['back_playlist'] = array(
        '#type' => 'item',
        '#markup' => '<a href="'.$base_url.'/manage_playlist/edit/'.$broadcast_id.'" class="btn btn-info pull-right" style="margin-top:25px;"><i class="fa fa-reply"></i> Quay lại</a>',
        '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>'
    );
    */
    $form['broadcast_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['info'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->body['und'][0]['value']) ? '<b>Mô tả : </b>'.$broadcast->body['und'][0]['value'] : '<b>Mô tả : </b>'.'Chưa có Mô tả cho lịch phát sóng. Vui lòng cập nhật!',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['start_time'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_start_time_playlist['und'][0]['value']) ? '<b>Từ ngày : </b><span class="label label-success">'.date('d-m-Y',$broadcast->field_start_time_playlist['und'][0]['value']) : '</span>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['end_time'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_start_time_playlist['und'][0]['value']) ? '<b>Tới ngày : </b><span class="label label-success">'.date('d-m-Y',$broadcast->field_end_time_playlist['und'][0]['value']) : '</span>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

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
    $form['list_broadcast']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Thiết lập thời gian cho Playlist</h2>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );


    $rows = array();
    //Load Playlist by broadcast_id
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->fields('bps',array('field_broadcast_playlist_start_value'));
    $query->condition('n.type','broadcast_playlist_time');
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
    $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
    $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');
    $query->orderBy('field_broadcast_playlist_start_value','ASC');

    $broadcast_time_result = $query->execute();
    if($broadcast_time_result->rowCount()){
        foreach($broadcast_time_result as $broadcast_time){
            $broadcast_time = node_load($broadcast_time->nid);

            $playlist = node_load($broadcast_time->field_playlist_broadcast_time_id['und'][0]['nid']);
            $order = 1;
            $number_slides = isset($playlist->field_slides_playlist['und']) ? count($playlist->field_slides_playlist['und']) : 0;
            //Tính thời gian của playlist
            $slide_ids = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : 0;
            $total_time = 0;
            if($slide_ids){
                foreach($slide_ids as $slide_id){
                    foreach($slide_id as $key=>$nid){
                        if($nid){
                            $slide = node_load($nid);
                            $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;

                        }
                    }
                }
            }

            $rows[] = array(
                $playlist->title,
                $number_slides,
                gmdate('H:i:s',$total_time),
                array(
                    'data' => array(
                        '#type' => 'textfield',
                        '#title' => 'Thời gian bắt đầu',
                        '#title_display' => 'invisible',
                        '#value' => isset($broadcast_time->field_broadcast_playlist_start['und'][0]['value']) ? date('H:i:s',$broadcast_time->field_broadcast_playlist_start['und'][0]['value']) : '',
                        '#name' => 'start_time['.$broadcast_time->nid.']',
                        '#size' => 5,
                        '#required'=> TRUE,
                        '#attributes' => array('placeholder'=>'00:00')
                    ),
                ),
                array(
                    'data' => array(
                        '#type' => 'textfield',
                        '#title' => 'Thời gian bắt đầu',
                        '#title_display' => 'invisible',
                        '#value' => isset($broadcast_time->field_broadcast_playlist_end['und'][0]['value']) ? date('H:i:s',$broadcast_time->field_broadcast_playlist_end['und'][0]['value']) : '',
                        '#name' => 'end_time['.$broadcast_time->nid.']',
                        '#size' => 5,
                        '#required'=> TRUE,
                        '#attributes' => array('placeholder'=>'00:00')
                    ),
                ),
            );
        }
    }


    $header = array(
        array('data' => 'Tên Playlist'),
        array('data' => 'Số Files'),
        array('data' => 'Thời lượng'),
        array('data' => 'Thời gian bắt đầu'),
        array('data' => 'Thời gian kết thúc'),
    );
    $form['list_broadcast']['body']['playlists'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['footer'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['broadcast_id'] = array(
        '#type' => 'hidden',
        '#value' => $broadcast_id
    );
    $form['list_broadcast']['footer']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Lưu lịch phát sóng',
        '#attributes' => array(
            'class' => array('btn','btn-info')
        ),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',

    );
    return $form;

}

function broadcast_set_time_playlist_form_submit($form,&$form_state){
    global $user,$base_url;
    $values = $form_state['input'];
    $start_time = $values['start_time'];
    $end_time = $values['end_time'];
    $broadcast_id = $values['broadcast_id'];
    $broadcast = node_load($broadcast_id);
    $order = 1;
    foreach($start_time as $broadcast_time_id=>$time){
        $broadcast_playlist_time = node_load($broadcast_time_id);
        $broadcast_playlist_time->field_broadcast_playlist_start['und'][0]['value'] = strtotime(date('Y-m-d',$broadcast->field_start_time_playlist['und'][0]['value']). ' '.$start_time[$broadcast_playlist_time->nid]);
        $broadcast_playlist_time->field_broadcast_playlist_end['und'][0]['value'] = strtotime(date('Y-m-d',$broadcast->field_start_time_playlist['und'][0]['value']). ' '.$end_time[$broadcast_playlist_time->nid]);
        $broadcast_playlist_time->status = 1;
        node_save($broadcast_playlist_time);
    }

    $form_state['redirect'] = $base_url.'/broadcast/view/playlist/'.$broadcast_id;

}

function broadcast_view_playlist_form($form,&$form_state,$broadcast_id){
    global $base_url;
    $broadcast = node_load($broadcast_id);
    $is_used = 0;
    if($broadcast->field_approval_playlist['und'][0]['value']==2){
        $is_used = 1;
    }

    $rows = array();
    $total_broadcast_time = 0;
    //Load Playlist by broadcast_id
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->fields('bps',array('field_broadcast_playlist_start_value'));
    $query->condition('n.type','broadcast_playlist_time');
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
    $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
    $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');

    $query->orderBy('field_broadcast_playlist_start_value','ASC');

    $broadcast_time_result = $query->execute();
    if($broadcast_time_result->rowCount()){
        $playlist_count = 1;
        foreach($broadcast_time_result as $broadcast_time){
            $broadcast_time = node_load($broadcast_time->nid);
            $playlist = node_load($broadcast_time->field_playlist_broadcast_time_id['und'][0]['nid']);
            $order = 1;
            $number_slides = isset($playlist->field_slides_playlist['und']) ? count($playlist->field_slides_playlist['und']) : 0;
            //Tính thời gian của playlist
            $slide_ids = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : 0;
            $total_time = 0;
            if($slide_ids){
                foreach($slide_ids as $slide_id){
                    foreach($slide_id as $key=>$nid){
                        if($nid){
                            $slide = node_load($nid);
                            $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;

                        }
                    }
                }
            }
            $total_broadcast_time += $total_time;
            if($is_used){
                $rows[] = array(
                    $playlist_count,
                    '<a href="'.$base_url.'/manage_playlist/view/'.$playlist->nid.'">'.$playlist->title.'</a>',
                    $number_slides,
                    gmdate('H:i:s',$total_time),
                    date('H:i:s', $broadcast_time->field_broadcast_playlist_start['und'][0]['value']),
                    date('H:i:s', $broadcast_time->field_broadcast_playlist_end['und'][0]['value']),
                );
            }else{
                $rows[] = array(
                    $playlist_count,
                    '<a href="'.$base_url.'/manage_playlist/view/'.$playlist->nid.'">'.$playlist->title.'</a>',
                    $number_slides,
                    gmdate('H:i:s',$total_time),
                    date('H:i:s', $broadcast_time->field_broadcast_playlist_start['und'][0]['value']),
                    date('H:i:s', $broadcast_time->field_broadcast_playlist_end['und'][0]['value']),
                    '<a href="'.$base_url.'/broadcast/delete/playlist/'.$broadcast_time->nid.'" class="btn btn-danger"><i class="fa fa-trash"></i> Xóa</a>'
                );
            }

            $playlist_count++;
        }
    }
    $form['broadcast_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
    );
    $form['broadcast_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3 class="col-md-10">'.$broadcast->title.'</h3> <a href="'.$base_url.'/broadcast/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>',

    );

    $form['broadcast_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $label_broadcast_status = get_label_broadcast_status($broadcast);
    $form['broadcast_info']['body']['broadcast_status'] = array(
        '#type' => 'item',
        '#markup' => '<b>Trạng thái : </b>'.$label_broadcast_status,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['info'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->body['und'][0]['value']) ? '<b>Mô tả : </b>'.$broadcast->body['und'][0]['value'] : '<b>Mô tả : </b>'.'Chưa có Mô tả cho lịch phát sóng. Vui lòng cập nhật!',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['start_time'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_start_time_playlist['und'][0]['value']) ? '<b>Từ ngày : </b><span class="label label-success">'.date('d-m-Y',$broadcast->field_start_time_playlist['und'][0]['value']) : '</span>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['end_time'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_end_time_playlist['und'][0]['value']) ? '<b>Tới ngày : </b><span class="label label-success">'.date('d-m-Y',$broadcast->field_end_time_playlist['und'][0]['value']) : '</span>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    /*
    $form['broadcast_info']['body']['total_files'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_broadcast_total_files['und'][0]['value']) ? '<b>Tổng số Files : </b><span class="label label-success">'.$broadcast->field_broadcast_total_files['und'][0]['value'].'</span>' : '0',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['broadcast_info']['body']['total_playlists'] = array(
        '#type' => 'item',
        '#markup' => isset($broadcast->field_broadcast_total_playlists['und'][0]['value']) ? '<b>Tổng số Playlist : </b><span class="label label-success">'.$broadcast->field_broadcast_total_playlists['und'][0]['value'].'</span>' : '0',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    */
    $form['broadcast_info']['body']['total_broadcast_time'] = array(
        '#type' => 'item',
        '#markup' => '<b>Tổng thời lượng : </b>'.gmdate('H:i:s',$total_broadcast_time),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );


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
    if($is_used){
        $add_more_playlist = '';
    }else{
        $add_more_playlist = '<a href="'.$base_url.'/broadcast/add/playlist/'.$broadcast_id.'" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Thêm Playlist</a>';
    }
    $form['list_broadcast']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3 class="col-md-10">Lịch phát sóng</h3> '.$add_more_playlist,
    );

    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    if($is_used){
        $header = array(
            array('data' => 'STT'),
            array('data' => 'Tên Playlist'),
            array('data' => 'Số Files'),
            array('data' => 'Thời lượng'),
            array('data' => 'Thời gian bắt đầu'),
            array('data' => 'Thời gian kết thúc'),
        );
    }else{
        $header = array(
            array('data' => 'STT'),
            array('data' => 'Tên Playlist'),
            array('data' => 'Số Files'),
            array('data' => 'Thời lượng'),
            array('data' => 'Thời gian bắt đầu'),
            array('data' => 'Thời gian kết thúc'),
            array('data' => 'Thao tác'),
        );
    }

    $form['list_broadcast']['body']['playlists'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );

   
    return $form;
}

function broadcast_delete_playlist_form($form,&$form_state,$broadcast_time_id){
    global $user,$base_url;
    if($broadcast_time_id){
        $broadcast_time = node_load($broadcast_time_id);
        $broadcast = node_load($broadcast_time->field_broadcast_playlist_time_id['und'][0]['nid']);
        $playlist = node_load($broadcast_time->field_playlist_broadcast_time_id['und'][0]['nid']);

            if(isset($broadcast->field_approval_playlist['und'][0]['value']) && ($broadcast->field_approval_playlist['und'][0]['value']==2)) {
                drupal_set_message('Bạn không thể xóa playlist trong lịch phát sóng đã được phê duyệt!','warning');
                drupal_goto('broadcast/view/playlist/'.$broadcast->nid);
            }else{

                $form['list_broadcast'] = array(
                    '#type' => 'container',
                    '#weight' => 2,
                    '#attributes' => array(
                        'class' => array('box', 'box-danger')
                    ),


                );

                $header_title = 'Bạn có chắc chắn muốn xóa playlist khỏi lịch phát sóng ?';

                $form['list_broadcast']['header'] = array(
                    '#type' => 'item',
                    '#markup' => '<h3 class="col-md-12">' . $header_title . '</h3>',
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
                    '#markup' => '<b>Tên Playlist : </b>' . $playlist->title,
                    '#prefix' => '<div class="col-md-6">',
                    '#suffix' => '</div>'
                );


                $form['list_broadcast']['footer'] = array(
                    '#type' => 'container',
                    '#prefix' => '<div class="box-footer width-border">',
                    '#suffix' => '</div>'
                );
                $form['list_broadcast']['footer']['buttons'] = array(
                    '#type' => 'container',
                    '#weight' => 3,
                    '#prefix' => '<div class="col-md-12">',
                    '#suffix' => '</div>'
                );

                $form['list_broadcast']['footer']['buttons']['delete_slide'] = array(
                    '#type' => 'submit',
                    '#value' => 'Xóa playlist',
                    '#attributes' => array(
                        'class' => array('btn', 'btn-danger')
                    ),
                    '#prefix' => '<div class="col-md-2  pull-right">',
                    '#suffix' => '</div>'
                );

                $form['list_broadcast']['broadcast_time_id'] = array(
                    '#type' => 'hidden',
                    '#value' => $broadcast_time_id
                );


                $form['list_broadcast']['footer']['buttons']['back_broadcast'] = array(
                    '#type' => 'item',
                    '#markup' => '<a href="' . $base_url . '/broadcast/view/playlist/' . $broadcast->nid . '" class="btn btn-info"><i class="fa fa-reply"></i> Quay lại </a>',
                    '#prefix' => '<div class="pull-right">',
                    '#suffix' => '</div>'
                );

                return $form;

            }
        }else{
            drupal_goto($base_url.'broadcast/list');
        }




}

function broadcast_delete_playlist_form_submit($form,&$form_state){
    global $base_url;
    
    $values = $form_state['values'];
    $broadcast_time_id = isset($values['broadcast_time_id']) ? $values['broadcast_time_id'] : 0;
    if($broadcast_time_id){
        $broadcast_time = node_load($broadcast_time_id);
        $broadcast = node_load($broadcast_time->field_broadcast_playlist_time_id['und'][0]['nid']);
        node_delete($broadcast_time_id);
        $form_state['redirect'] = $base_url.'/broadcast/view/playlist/'.$broadcast->nid;
    }

}

function broadcast_delete_form($form,&$form_state,$broadcast_id){
    global $user,$base_url;
    if($broadcast_id){
        $broadcast = node_load($broadcast_id);
        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'

        );
        if(isset($broadcast->field_approval_playlist['und'][0]['value']) && ($broadcast->field_approval_playlist['und'][0]['value']==2)){
            $header_title = 'Lịch phát sóng đã được phê duyệt nên bạn không thể xóa!';
        }else{
            $header_title = 'Bạn có chắc chắn muốn xóa lịch phát sóng ?';
        }
        $form['list_broadcast']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-12">'.$header_title.'</h3>',
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
            '#markup' => '<b>Lịch phát sóng : </b>' .$broadcast->title,
            '#prefix' => '<div class="col-md-6">',
            '#suffix' => '</div>'
        );


        $form['list_broadcast']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['footer']['buttons'] = array(
            '#type' => 'container',
            '#weight' => 3,
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        if($broadcast->field_approval_playlist['und'][0]['value']!=2) {
            $form['list_broadcast']['footer']['buttons']['delete_slide'] = array(
                '#type' => 'submit',
                '#value' => 'Xóa lịch phát sóng',
                '#attributes' => array(
                    'class' => array('btn', 'btn-danger')
                ),
                '#prefix' => '<div class="col-md-2  pull-right">',
                '#suffix' => '</div>'
            );

            $form['list_broadcast']['broadcast_id'] = array(
                '#type' => 'hidden',
                '#value' => $broadcast_id
            );
        }
        $form['list_broadcast']['footer']['buttons']['back_broadcast'] = array(
            '#type' => 'item',
            '#markup' => '<a href="'.$base_url.'/broadcast/list" class="btn btn-info"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="pull-right">',
            '#suffix' => '</div>'
        );

        return $form;
    }else{
        drupal_goto('broadcast/list');
        $form_state['rebuild'] = true;
    }
}
function broadcast_delete_form_submit($form,&$form_state){
    $values = $form_state['values'];
    if(isset($values['broadcast_id']) && $values['broadcast_id']){
        $broadcast_id = $values['broadcast_id'];
        $broadcast = node_load($broadcast_id);

        //Load Playlist by broadcast_id
        $query = db_select('node','n');
        $query->fields('n',array('nid'));
        $query->fields('bps',array('field_broadcast_playlist_start_value'));
        $query->condition('n.type','broadcast_playlist_time');
        $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
        $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
        $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');
        $query->orderBy('field_broadcast_playlist_start_value','ASC');
        $broadcast_time_result = $query->execute();
        if($broadcast_time_result->rowCount()){
            foreach($broadcast_time_result as $broadcast_time_id){
                node_delete($broadcast_time_id->nid);
            }
        }

        node_delete($broadcast_id);
        drupal_set_message('Xóa lịch phát sóng thành công!');

    }
    $form_state['redirect'] = 'broadcast/list';

}

function load_playlist_search($broadcast_id =0,$playlist_category='',$playlist_name='',$playlist_ids = array()){
    global $base_url,$user;
    $rows = array();

    $query = db_select('node', 'n');
    $query->fields('n', array('nid'));
    $query->condition('n.type', 'playlist', '=');
    if($playlist_category){
        $query->leftJoin('field_data_field_playlist_category','fpc','fpc.entity_id=n.nid');
        $query->condition('field_playlist_category_tid',$playlist_category,'=');
    }
    if($playlist_name){
        $query->condition('title', '%' . db_like($playlist_name) . '%', 'LIKE');
    }
    if($playlist_ids){
        foreach($playlist_ids as $playlist_id){
            $query->condition('n.nid',$playlist_id,'<>');
        }

    }
    //$query->leftJoin('field_data_field_playlist_default','fpd','fpd.entity_id=n.nid');
    //$query->condition('field_playlist_default_value',2);
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
            //Calculate time of playlist
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

            $status = $status_playlist_values[$node->field_status_playlist['und'][0]['value']];
            if (isset($node->field_playlist_category['und'][0]['tid'])) {
                $term = taxonomy_term_load($node->field_playlist_category['und'][0]['tid']);
                $category_name = $term->name;
            } else {
                $category_name = '';
            }
            $add_playlist = '<div id="tools_broadcast_'.$node->nid.'"><a href="#" class="btn btn-success add-playlist" data-broadcast="'.$broadcast_id.'" data-playlist="'.$node->nid.'"><i class="fa fa-plus"></i> Chọn</a></div>';
            $remove_playlist = '<div id="tools_broadcast_remove_'.$node->nid.'" style="display:none"><a href="#" class="btn btn-danger remove-playlist" data-broadcast="'.$broadcast_id.'" data-playlist="'.$node->nid.'"><i class="fa fa-trash"></i> Xóa</a></div>';
            $rows[$node->nid] = array(
                'name'=>'<a href="' . $base_url . '/manage_playlist/view/' . $node->nid . '">' . $node->title . '</a>',
                'number'=>$number_slides,
                'duration' => gmdate('H:i:s',$total_time),
                'category'=>$category_name,
                'action' =>$add_playlist . $remove_playlist
            );
            $i++;
        }
    }

    return $rows;

}

function device_update_mac_import_form($form, & $form_state){
    $form = array();
    $form['general'] = array(
        '#type' => 'fieldset',
        '#title' => 'Import danh sách MAC từ file Excel',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    $form['general']['file'] = array(
        '#type' => 'file',
        '#title' => 'Chọn file Excel để Import :',
        '#description' => t('Chỉ chấp nhận file định dạng Excel: xls'),
    );
    $form['general']['file_excel'] = array(
        '#type' => 'submit',
        '#value' => 'Import danh sách MAC',
        '#attributes' => array('class' => array('btn-success pull-right')),
    );
    return $form;


}

function device_mac_import_form($form, & $form_state){
    $form = array();
    $form['general'] = array(
        '#type' => 'fieldset',
        '#title' => 'Import danh sách MAC từ file Excel',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    $form['general']['file'] = array(
        '#type' => 'file',
        '#title' => 'Chọn file Excel để Import :',
        '#description' => t('Chỉ chấp nhận file định dạng Excel: xls'),
    );
    $form['general']['file_excel'] = array(
        '#type' => 'submit',
        '#value' => 'Import danh sách MAC',
        '#attributes' => array('class' => array('btn-success pull-right')),
    );
    return $form;


}
function device_mac_import_form_validate ($form,&$form_state){
    $file = file_save_upload('file', array(
        'file_validate_is_excel' => array(),
        'file_validate_extensions' => array('xls'),
    ));

    if ($file) {
        if ($file = file_move($file, 'public://')) {
            $form_state['values']['file'] = $file;
        }
        else {
            form_set_error('file', t('Có lỗi trong quá trình tải file lên máy chủ.'));
        }
    }
    else {
        form_set_error('file', t('File chưa tải lên máy chủ.'));
    }

}

function device_store_import_form($form, &$form_state){
    $form = array();
    $form['general'] = array(
        '#type' => 'fieldset',
        '#title' => 'Import danh sách Cửa hàng từ file Excel',
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    $form['general']['file'] = array(
        '#type' => 'file',
        '#title' => 'Chọn file Excel để Import :',
        '#description' => t('Chỉ chấp nhận file định dạng Excel: xls'),
    );
    $form['general']['file_excel'] = array(
        '#type' => 'submit',
        '#value' => 'Import danh sách Cửa hàng',
        '#attributes' => array('class' => array('btn-success pull-right')),
    );
    return $form;
}

function device_store_import_form_validate ($form,&$form_state){
    $file = file_save_upload('file', array(
        'file_validate_is_excel' => array(),
        'file_validate_extensions' => array('xls'),
    ));

    if ($file) {
        if ($file = file_move($file, 'public://')) {
            $form_state['values']['file'] = $file;
        }
        else {
            form_set_error('file', t('Có lỗi trong quá trình tải file lên máy chủ.'));
        }
    }
    else {
        form_set_error('file', t('File chưa tải lên máy chủ.'));
    }

}
function device_store_import_form_submit($form, & $form_state)
{
    module_load_include('inc', 'phpexcel');
    global $user;
    /*
    $vocabulary = taxonomy_vocabulary_machine_name_load('store');
    foreach (taxonomy_get_tree($vocabulary->vid) as $term) {
        taxonomy_term_delete($term->tid);
    }
    die;
    */
    $values = $form_state['values'];
    $uri = $values['file']->uri;

    $result = phpexcel_import($uri);
    $infos = $result[0];

    foreach($infos as $info) {

        $query = db_select('taxonomy_term_data', 'ttd');
        $query->fields('ttd', array('tid', 'name'));
        $query->condition('ttd.name',$info[1]);
        $result = $query->execute()->fetchAssoc();

        if ($result) {
            $store_term = taxonomy_term_load($result['tid']);
            $store_term->description = $info[6];
            $store_term->field_store_sap['und'][0]['value'] = $info[0];
            taxonomy_term_save($store_term);
        }else{
            //Tạo mới cửa hàng
            $voca = taxonomy_vocabulary_machine_name_load('store');
            $new_store = new stdClass();
            $new_store->name = $info[1];
            $new_store->vid = $voca->vid;
            $new_store->parent = 0;

            //Update Vùng miền
            if($info[2]=='Miền Bắc'){
                $new_store->field_region_store_id['und'][0]['tid'] = 168;
            }else{
                $new_store->field_region_store_id['und'][0]['tid'] = 170;
            }

            //Update Tỉnh/Thành
            if($info[3] == 'HÀ NỘI'){
                $new_store->field_province_store_id['und'][0]['tid'] = 171;
            }

            if($info[3] == 'HCM'){
                $new_store->field_province_store_id['und'][0]['tid'] = 186;
            }
            if($info[3] == 'ĐỒNG NAI'){
                $new_store->field_province_store_id['und'][0]['tid'] = 182;
            }
            if($info[3] == 'ĐÀ NẴNG'){
                $new_store->field_province_store_id['und'][0]['tid'] = 181;
            }
            if($info[3] == 'NHA TRANG'){
                $new_store->field_province_store_id['und'][0]['tid'] = 188;
            }
            if($info[3] == 'CẦN THƠ'){
                $new_store->field_province_store_id['und'][0]['tid'] = 184;
            }
            if($info[3] == 'HƯNG YÊN'){
                $new_store->field_province_store_id['und'][0]['tid'] = 187;
            }
            if($info[3] == 'BÌNH DƯƠNG'){
                $new_store->field_province_store_id['und'][0]['tid'] = 183;
            }
            if($info[3] == 'QUẢNG NINH'){
                $new_store->field_province_store_id['und'][0]['tid'] = 189;
            }
            if($info[3] == 'HẢI PHÒNG'){
                $new_store->field_province_store_id['und'][0]['tid'] = 185;
            }
            //Update company
            $new_store->field_company_store_id['und'][0]['tid'] = 100;
            $new_store->description = $info[6];
            $new_store->field_store_sap['und'][0]['value'] = $info[0];
            taxonomy_term_save($new_store);
        }

    }


}

function device_mac_import_form_submit($form, & $form_state){
    module_load_include('inc', 'phpexcel');
    global $user;
    /*
    $vocabulary = taxonomy_vocabulary_machine_name_load('mac_address');
    foreach (taxonomy_get_tree($vocabulary->vid) as $term) {
        taxonomy_term_delete($term->tid);
    }
    */


    $values = $form_state['values'];
    $uri = $values['file']->uri;

    $result = phpexcel_import($uri);

    $macs = $result[0];

    if (is_array($result)) {
        foreach ($macs as $mac){
                    $query = db_select('taxonomy_term_data', 'ttd');
                    $query->fields('ttd', array('tid','name'));
                    $query->condition('ttd.name',$mac[1]);
                    $result = $query->execute()->fetchAssoc();
                    if($result){
                        $mac_term = taxonomy_term_load($result['tid']);
                        $mac_name = $result['name'];
                        if (substr($mac_name, 0, 2) == 'F0') {
                            $mac_term->field_mac_region_id['und'][0]['tid'] = 61;
                        } else {
                            $mac_term->field_mac_region_id['und'][0]['tid'] = 51;
                        }
                        $mac_term->field_mac_company_id['und'][0]['tid'] = 49;
                        $mac_term->field_mac_number['und'][0]['value'] = $mac[0];
                        //$mac_term->field_mac_sap['und'][0]['value'] = $mac[2];
                        $mac_term->field_device_status['und'][0]['value'] = 2;
                        taxonomy_term_save($mac_term);
                    }else{
                        //update new MAC
                        $voca = taxonomy_vocabulary_machine_name_load('mac_address');
                        $new_mac = new stdClass();
                        if(substr($mac[1],0,2)=='F0'){
                            $mac_term->field_mac_region_id['und'][0]['tid'] = 61;
                        }else{
                            $mac_term->field_mac_region_id['und'][0]['tid'] = 51;
                        }
                        $new_mac->name = $mac[1];
                        $new_mac->vid = $voca->vid;
                        $new_mac->parent = 0;
                        $new_mac->field_device_status['und'][0]['value'] = 2;
                        //$new_mac->field_mac_last_login['und'][0]['value'] = strtotime(date("Y-m-d H:i:s", time()));
                        //$new_mac->field_mac_ip['und'][0]['value'] = ip();
                        $new_mac->field_mac_number['und'][0]['value'] = $mac[0];
                        //$new_mac->field_mac_sap['und'][0]['value'] = $mac[2];
                        taxonomy_term_save($new_mac);
                    }



        }

    }
    else {
        drupal_set_message(t("Oops ! Có lỗi trong quá trình đọc file!"), 'error');
    }


}

function get_label_broadcast_status($broadcast){
    $status_broadcast = field_info_field('field_approval_playlist');
    $status_broadcast_values = $status_broadcast['settings']['allowed_values'];
    $label_status_broadcast = '';
    if(isset($broadcast->field_approval_playlist['und'][0]['value']) && $broadcast->field_approval_playlist['und'][0]['value']) {
        switch ($broadcast->field_approval_playlist['und'][0]['value']) {
            case 1:
                $label_status_broadcast = '<span class="label label-warning">' . $status_broadcast_values[$broadcast->field_approval_playlist['und'][0]['value']] . '</span>';
                break;
            case 2:
                $label_status_broadcast = '<span class="label label-success">' . $status_broadcast_values[$broadcast->field_approval_playlist['und'][0]['value']] . '</span>';
                break;
            case 3:
                $label_status_broadcast = '<span class="label label-danger">' . $status_broadcast_values[$broadcast->field_approval_playlist['und'][0]['value']] . '</span>';
                break;

        }
    }
    return $label_status_broadcast;
}

function broadcast_review_form($form,&$form_state){
    global $user,$base_url;
    $form = array();
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $store_id = isset($_GET['store_id']) ? $_GET['store_id'] : 0;
    $status = isset($_GET['status']) ? $_GET['status'] : 0;
    $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : 0;
    $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;

    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Từ ngày'),
        array('data'=>'Tới ngày'),
        array('data'=>'Lịch phát sóng'),
        array('data' => 'Trạng thái'),
        array('data' => 'Công ty'),
        array('data' => 'Vùng miền'),
        array('data' => 'Tỉnh/Thành'),
        array('data' => 'Cửa hàng'),
        array('data' =>'Người duyệt'),
        array('data' => 'Ngày duyệt'),
        array('data'=>'Thao tác'),

    );
    $number_per_page = 50;
    $result = load_broadcast_review($company_id,$region_id,$province_id,$store_id,$status,$start_date,$end_date,$header,$number_per_page);
    $rows = $result['rows'];
    $total_result = $result['total_result'];

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
        '#type' => 'item',
        '#markup' =>'<h3>Danh sách lịch phát sóng</h3>',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $company_options = load_voca_info('company');
    $form['list_broadcast']['body']['broadcast_area']['broadcast_company'] = array(
        '#type' => 'select',
        '#title' => 'Công ty',
        '#options' => $company_options,
        '#default_value' => $company_id,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#ajax' => array(
            'callback' => 'region_select_callback',
            'wrapper' => 'region-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
        '#validated' => TRUE,
    );

    if($company_id){
        $region_options = load_voca_info('region',array('company_id'=>$company_id));
    }else{
        $region_options = array(0=>t('Chọn vùng miền'));
    }
    $form['list_broadcast']['body']['broadcast_area']['broadcast_region'] = array(
        '#type' => 'select',
        '#title' => 'Vùng miền',
        '#options' => $region_options,
        '#default_value' => $region_id,
        '#prefix' => '<div id="region-wrapper" class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'province_select_callback',
            'wrapper' => 'province-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );
    if($region_id){
        $province_options = load_voca_info('province',array('company_id'=>$company_id,'region_id'=>$region_id));
    }else{
        $province_options = array(0=>('Chọn tỉnh/thành'));
    }
    $form['list_broadcast']['body']['broadcast_area']['broadcast_province'] = array(
        '#type' => 'select',
        '#title' => 'Tỉnh / Thành',
        '#options' => $province_options,
        '#default_value' => $province_id,
        '#prefix' => '<div id="province-wrapper" class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'store_select_callback',
            'wrapper' => 'store-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );

    if($province_id){
        $store_options = load_voca_info('store',array('company_id'=>$company_id,'region_id'=>$region_id,'province_id'=>$province_id));
    }else{
        $store_options = array(0=>'Chọn cửa hàng');
    }

    $form['list_broadcast']['body']['broadcast_area']['broadcast_store'] = array(
        '#type' => 'select',
        '#title' => 'Cửa hàng',
        '#options' => $store_options,
        '#default_value' => $store_id,
        '#prefix' => '<div class="col-md-3" id="store-wrapper">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );


    $form['list_broadcast']['body']['broadcast_area']['start_date'] = array(
        '#type' => 'date_popup',
        //'#timepicker' => 'timepicker',
        '#title' => 'Từ ngày',
        '#date_label_position' => 'none',
        '#date_format' => 'd/m/Y',
        '#default_value' => $start_date,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#size' => 30
    );
    $form['list_broadcast']['body']['broadcast_area']['end_date'] = array(
        '#type' => 'date_popup',
        //'#timepicker' => 'timepicker',
        '#title' => 'Tới ngày',
        '#date_label_position' => 'none',
        '#date_format' => 'd/m/Y',
        '#default_value' => $end_date,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#size' => 30
    );

    $status_broadcast = field_info_field('field_approval_playlist');
    $status_broadcast_values = $status_broadcast['settings']['allowed_values'];

    $review_options = array('0'=> 'Chọn trạng thái');
    if($status_broadcast_values){
        foreach($status_broadcast_values as $key=>$value){
            $review_options[$key] = $value;
        }
    }
    $form['list_broadcast']['body']['broadcast_area']['status'] = array(
        '#type' => 'select',
        '#title' => 'Trạng thái',
        '#options' => $review_options,
        '#default_value' => $status,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,

    );

    $form['list_broadcast']['body']['buttons'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12" style="padding-right:0px;">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body']['buttons']['search_button'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#weight' => 3,
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>'

    );
    $message = '<h4>Tổng số lịch phát sóng là : '.$total_result.'</h4>';

    $form['list_broadcast']['body']['search_summary'] = array(
        '#type' => 'item',
        '#weight' => 2,
        '#markup' => '<div class="callout callout-info">'.$message.'</div>',
        '#prefix' => '<div class="col-md-12" style="margin-top : 30px;">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['result'] = array(
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

function broadcast_review_form_submit($form,&$form_state){
    global $base_url;
    $values = $form_state['values'];

    $company_id = isset($values['broadcast_company']) ? $values['broadcast_company'] : 0;
    $region_id = isset($values['broadcast_region']) ? $values['broadcast_region'] : 0;
    $province_id = isset($values['broadcast_province']) ? $values['broadcast_province'] : 0;
    $store_id = isset($values['broadcast_store']) ? $values['broadcast_store'] : 0;
    $status = isset($values['status']) ? $values['status'] : 0;
    $start_date = isset($values['start_date']) ? $values['start_date'] : 0;
    $end_date = isset($values['end_date']) ? $values['end_date'] : 0;

    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
        'store_id' => $store_id,
        'status' => $status,
        'page' => isset($values['page']) ? $values['page'] : 0,
        'start_date' => $start_date,
        'end_date' => $end_date
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/broadcast/review/list?'.$query_string;
}

function load_broadcast_review($company_id=0,$region_id=0,$province_id=0,$store_id=0,$status=0,$start_date=0,$end_date=0,$header,$number_per_page=50){
    global $base_url;
    $rows = array();
    $query = db_select('node', 'n')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit($number_per_page);
    $query->fields('n',array('nid'));
    $query->condition('n.type','broadcast_playlist','=');
    if($company_id){
        $query->leftJoin('field_data_field_company_id_playlist','cip','cip.entity_id=n.nid');
        $query->condition('field_company_id_playlist_tid',$company_id);
    }
    if($region_id){
        $query->leftJoin('field_data_field_region_id_playlist','rip','rip.entity_id=n.nid');
        $query->condition('field_region_id_playlist_tid',$region_id);
    }
    if($province_id){
        $query->leftJoin('field_data_field_province_id_playlist','pip','pip.entity_id=n.nid');
        $query->condition('field_province_id_playlist_tid',$province_id);
    }
    if($store_id){
        $query->leftJoin('field_data_field_store_id_playlist','sip','sip.entity_id=n.nid');
        $query->condition('field_store_id_playlist_tid',$store_id);
    }
    if($status){
        $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
        $query->condition('field_approval_playlist_value',$status);
    }
    if($start_date){
        $query->leftJoin('field_data_field_start_time_playlist','stp','stp.entity_id=n.nid');
        $query->condition('field_start_time_playlist_value',strtotime($start_date),'<=');
    }
    if($end_date){
        $query->leftJoin('field_data_field_end_time_playlist','etp','etp.entity_id=n.nid');
        $query->condition('field_end_time_playlist_value',strtotime($end_date),'>=');
    }
    $query->orderBy('n.created','DESC');
    $query->condition('n.status',1,'=');

    $num_rows = $query->countQuery()->execute()->fetchField();
    $result = $query->execute();
    if($result->rowCount()){
        $i = 1;
        foreach($result as $row){
            $broadcast = node_load($row->nid);

            //Load Playlist by broadcast_id
            $query = db_select('node','n');
            $query->fields('n',array('nid'));
            $query->fields('bps',array('field_broadcast_playlist_start_value'));
            $query->condition('n.type','broadcast_playlist_time');
            $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
            $query->condition('field_broadcast_playlist_time_id_nid',$row->nid);
            $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');
            $query->orderBy('field_broadcast_playlist_start_value','ASC');
            $broadcast_time_result = $query->execute();
            if($broadcast_time_result->rowCount()){
                $number_playlist = $broadcast_time_result->rowCount();
            }else{
                $number_playlist = 0;
            }

            $term_company = taxonomy_term_load($broadcast->field_company_id_playlist['und'][0]['tid']);
            $company_name = $term_company->name;
            if(isset($broadcast->field_region_id_playlist['und'][0]['tid']) && $broadcast->field_region_id_playlist['und'][0]['tid']){
                $term_region = taxonomy_term_load($broadcast->field_region_id_playlist['und'][0]['tid']);
                $region_name = $term_region->name;
            }else{
                $region_name = '';
            }
            if(isset($broadcast->field_province_id_playlist['und'][0]['tid']) && $broadcast->field_province_id_playlist['und'][0]['tid']){
                $term_province = taxonomy_term_load($broadcast->field_province_id_playlist['und'][0]['tid']);
                $province_name = $term_province->name;
            }else{
                $province_name = '';
            }
            if(isset($broadcast->field_store_id_playlist['und'][0]['tid']) && $broadcast->field_store_id_playlist['und'][0]['tid']){
                $term_store = taxonomy_term_load($broadcast->field_store_id_playlist['und'][0]['tid']);
                $store_name = $term_store->name;
            }else{
                $store_name = '';
            }

            //Lấy trạng thái lịch phát sóng
            $label_status_broadcast = get_label_broadcast_status($broadcast);
            $reviewer = user_load($broadcast->uid);

            $rows[] = array(
                $i,
                date('d/m/Y',$broadcast->field_start_time_playlist['und'][0]['value']),
                date('d/m/Y',$broadcast->field_end_time_playlist['und'][0]['value']),
                '<a href="'.$base_url.'/broadcast/view/playlist/'.$broadcast->nid.'">'.$broadcast->title.'</a>',
                $label_status_broadcast,
                $company_name,
                $region_name,
                $province_name,
                $store_name,
                $reviewer->name,
                date('d/m/Y H:i:s',$broadcast->changed),
                '<div class="btn-group">
                    <button type="button" class="btn btn-success">Thao tác</button>
                    <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                        <span class="caret"></span>
                        <span class="sr-only">Toggle Dropdown</span>
                    </button>
                    <ul class="dropdown-menu" role="menu">
                        <li><a href="'.$base_url.'/broadcast/publish/'.$broadcast->nid.'"><i class="fa fa-edit"></i> Phê duyệt</a></li>
                        <li><a href="'.$base_url.'/broadcast/view/history/'.$broadcast->nid.'"><i class="fa fa-info"></i> Lịch sử</a></li>
                    </ul>
                </div>'
            );
            $i++;
        }
    }
    $return['rows'] = $rows;
    $return['total_result'] = $num_rows;
    return $return;
}


function broadcast_review_detail_form($form,&$form_state,$broadcast_id){
    global $base_url;
    $broadcast = node_load($broadcast_id);
    //Check broadcast review
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->condition('n.type','broadcast_review');
    $query->leftJoin('field_data_field_broadcast_review_id','br','br.entity_id=n.nid');
    $query->condition('field_broadcast_review_id_nid',$broadcast_id);
    $query->range(0,1);
    $broadcast_review_result = $query->execute()->fetchAssoc();

    if($broadcast_review_result['nid']){
        $broadcast_review = node_load($broadcast_review_result['nid']);
    }


    $form['list_broadcast_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
    );
    $form['list_broadcast_info']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast_info']['header']['header-title'] = array(
        '#type' => 'item',
        '#markup' =>'<h3>'.$broadcast->title.'</h3>',

    );
    $form['list_broadcast_info']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast_info']['body']['broadcast-date-start'] = array(
        '#type' => 'item',
        '#markup' => '<b>Từ ngày </b><span class="label label-success">'.date('d/m/Y',$broadcast->field_start_time_playlist['und'][0]['value']).'</span>',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast_info']['body']['broadcast-date-end'] = array(
        '#type' => 'item',
        '#markup' => '<b>Tới ngày </b> <span class="label label-success">'.date('d/m/Y',$broadcast->field_end_time_playlist['und'][0]['value']).'</span>',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast_info']['body']['publish_option'] = array(
        '#type' => 'fieldset',
        '#title' => t('Xét duyệt'),
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>',
    );
    $publish_options = array();
    $broadcast_review_status = field_info_field('field_broadcast_review_status');
    $broadcast_review_status_value = $broadcast_review_status['settings']['allowed_values'];
    foreach($broadcast_review_status_value as $key=>$value){
        $publish_options[$key] = $value;
    }

    $form['list_broadcast_info']['body']['publish_option']['review_status'] = array(
        '#type' => 'select',
        '#title' => 'Trạng thái',
        '#options' => $publish_options,
        '#weight' => 2,
        '#default_value' => (isset($broadcast_review)) ? $broadcast_review->field_broadcast_review_status['und'][0]['value'] : 0,
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast_info']['body']['publish_option']['comment'] = array(
        '#type' => 'textarea',
        '#title' => 'Nhận xét',
        '#weight' => 2,
        '#default_value' => isset($broadcast_review) ? $broadcast_review->body['und'][0]['value'] : '',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast_info']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>',
        '#weight' => 2
    );
    $form['list_broadcast_info']['broadcast_id'] = array(
        '#type' => 'hidden',
        '#value' => $broadcast_id
    );
    $form['list_broadcast_info']['footer']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'Phê duyệt',
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-info')
        ),
    );

    $rows = array();
    $total_broadcast_time = 0;
    //Load Playlist by broadcast_id
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->fields('bps',array('field_broadcast_playlist_start_value'));
    $query->condition('n.type','broadcast_playlist_time');
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
    $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
    $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');

    $query->orderBy('field_broadcast_playlist_start_value','ASC');

    $broadcast_time_result = $query->execute();
    if($broadcast_time_result->rowCount()){
        $playlist_count = 1;
        foreach($broadcast_time_result as $broadcast_time){
            $broadcast_time = node_load($broadcast_time->nid);
            $playlist = node_load($broadcast_time->field_playlist_broadcast_time_id['und'][0]['nid']);
            $order = 1;
            $number_slides = isset($playlist->field_slides_playlist['und']) ? count($playlist->field_slides_playlist['und']) : 0;
            //Tính thời gian của playlist
            $slide_ids = isset($playlist->field_slides_playlist['und']) ? $playlist->field_slides_playlist['und'] : 0;
            $total_time = 0;
            if($slide_ids){
                foreach($slide_ids as $slide_id){
                    foreach($slide_id as $key=>$nid){
                        if($nid){
                            $slide = node_load($nid);
                            $total_time += isset($slide->field_slide_duration['und'][0]['value']) ? $slide->field_slide_duration['und'][0]['value'] : 0;

                        }
                    }
                }
            }
            $total_broadcast_time += $total_time;
            $rows[] = array(
                $playlist_count,
                '<a href="'.$base_url.'/manage_playlist/view/'.$playlist->nid.'">'.$playlist->title.'</a>',
                $number_slides,
                gmdate('H:i:s',$total_time),
                date('H:i:s', $broadcast_time->field_broadcast_playlist_start['und'][0]['value']),
                date('H:i:s', $broadcast_time->field_broadcast_playlist_end['und'][0]['value'])
            );
            $playlist_count++;
        }
    }
    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        )
    );
    $form['list_broadcast']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h2>Thông tin chi tiết Lịch phát sóng</h2>',
    );

    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );

    $header = array(
        array('data' => 'STT'),
        array('data' => 'Tên Playlist'),
        array('data' => 'Số Files'),
        array('data' => 'Thời lượng'),
        array('data' => 'Thời gian bắt đầu'),
        array('data' => 'Thời gian kết thúc'),
    );
    $form['list_broadcast']['body']['playlists'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    return $form;
}



function broadcast_review_detail_form_submit($form,&$form_state){
    global $user,$base_url;
    $values = $form_state['values'];
    $review_status = isset($values['review_status']) ? $values['review_status'] : 0;
    $comment = isset($values['comment']) ? $values['comment'] : '';
    $broadcast_id = isset($values['broadcast_id']) ? $values['broadcast_id'] : 0;

    if($broadcast_id){
        $broadcast = node_load($broadcast_id);

        $review = new stdClass();
        $review->type = 'broadcast_review';
        $review->uid = $user->uid;
        $review->title = 'Xét duyệt lịch phát sóng : ' . $broadcast->title;
        $review->language = LANGUAGE_NONE;
        $review->body['und'][0]['value'] = $comment;
        $review->field_broadcast_review_id['und'][0]['nid'] = $broadcast_id;
        $review->field_broadcast_review_status['und'][0]['value'] = $review_status;
        node_save($review);

        if($review->nid){
            $broadcast->field_approval_playlist['und'][0]['value'] = $review_status;
            node_save($broadcast);
            drupal_set_message('Duyệt lịch phát sóng : '.$broadcast->title.' thành công!');
        }else{
            drupal_set_message('Có lỗi trong quá trình Duyệt lịch phát sóng : '.$broadcast->title.'.Vui lòng liên hệ quản trị hệ thống','error');
        }
        $form_state['redirect'] = $base_url.'/broadcast/review/list';
    }

}

function broadcast_review_history_form($form,&$form_state,$broadcast_id){
    global $user,$base_url;
    $rows = array();
    if($broadcast_id){
        $query = db_select('node','n');
        $query->fields('n',array('nid'));
        $query->condition('n.type','broadcast_review');
        $query->leftJoin('field_data_field_broadcast_review_id','bri','bri.entity_id=n.nid');
        $query->condition('field_broadcast_review_id_nid',$broadcast_id);
        $query->orderBy('n.created','DESC');

        $broadcast_reviews = $query->execute();

        if($broadcast_reviews->rowCount()){
            $count = 1;
            foreach($broadcast_reviews as $review_id){
                $broadcast_review = node_load($review_id->nid);
                $broadcast = node_load($broadcast_review->field_broadcast_review_id['und'][0]['nid']);

                $status_broadcast = field_info_field('field_broadcast_review_status');
                $status_broadcast_values = $status_broadcast['settings']['allowed_values'];
                $label_status_broadcast = '';
                if(isset($broadcast_review->field_broadcast_review_status['und'][0]['value']) && $broadcast_review->field_broadcast_review_status['und'][0]['value']) {
                    switch ($broadcast_review->field_broadcast_review_status['und'][0]['value']) {
                        case 1:
                            $label_status_broadcast = '<span class="label label-warning">' . $status_broadcast_values[$broadcast_review->field_broadcast_review_status['und'][0]['value']] . '</span>';
                            break;
                        case 2:
                            $label_status_broadcast = '<span class="label label-success">' . $status_broadcast_values[$broadcast_review->field_broadcast_review_status['und'][0]['value']] . '</span>';
                            break;
                        case 3:
                            $label_status_broadcast = '<span class="label label-danger">' . $status_broadcast_values[$broadcast_review->field_broadcast_review_status['und'][0]['value']] . '</span>';
                            break;

                    }
                }

                $user=user_load($broadcast_review->uid);
                $rows[] = array(
                    $count,
                    $broadcast->title,
                    $label_status_broadcast,
                    $broadcast_review->body['und'][0]['value'],
                    date('H:i:s d/m/Y',$broadcast_review->created),
                    $user->name

                );
                $count++;
            }
        }
    }
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Lịch phát sóng'),
        array('data' => 'Trạng thái'),
        array('data' => 'Nhận xét'),
        array('data' => 'Ngày phê duyệt'),
        array('data' => 'Người phê duyệt'),
    );
    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        )
    );
    $form['list_broadcast']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h2 class="col-md-10">Lịch sử phê duyệt lịch phát sóng</h2><a href="'.$base_url.'/broadcast/review/list" class="pull-right btn btn-info" style="margin-top:25px;"><i class="fa fa-reply"></i> Quay lại</a>',
    );

    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['reviews'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có phê duyệt lịch phát sóng nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    return $form;
}







function load_broadcast_time($broadcast_id){

    //Load Playlist by broadcast_id
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->fields('bps',array('field_broadcast_playlist_start_value'));
    $query->condition('n.type','broadcast_playlist_time');
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
    $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
    $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');

    $query->orderBy('field_broadcast_playlist_start_value','ASC');

    $broadcast_time_result = $query->execute();
    if($broadcast_time_result->rowCount()) {
        foreach ($broadcast_time_result as $broadcast_time_id) {
            $broadcast_time[] = node_load($broadcast_time_id->nid);
        }
    }else{
        $broadcast_time = 0;
    }
    return $broadcast_time;
}

function device_mac_list_form($form,&$form_state){

    $query = db_select('taxonomy_term_data', 'ttd');
    $query->fields('ttd', array('tid'));
    $query->condition('ttd.name','1');

    $result = $query->execute();

}


function broadcast_add_playlist_ajax(){
    global $user;
    $broadcast_id = isset($_POST['broadcast_id']) ? $_POST['broadcast_id'] : 0;
    $playlist_id = isset($_POST['playlist_id']) ? $_POST['playlist_id'] : 0;
    if($broadcast_id && $playlist_id){

        $broadcast = node_load($broadcast_id);
        $order = 1;
        //Tạo Broadcast Playlist Time
        $broadcast_playlist_time = new stdClass();
        $broadcast_playlist_time->type = 'broadcast_playlist_time';
        $broadcast_playlist_time->title = 'Playlist cho lịch phát sóng : '.$broadcast->title;
        $broadcast_playlist_time->field_broadcast_playlist_time_id['und'][0]['nid'] = $broadcast_id;
        $broadcast_playlist_time->field_playlist_broadcast_time_id['und'][0]['nid'] = $playlist_id;
        $broadcast_playlist_time->field_broadcast_playlist_order['und'][0]['value'] = $order;
        $broadcast_playlist_time->language = LANGUAGE_NONE;
        $broadcast_playlist_time->uid = $user->uid;
        $broadcast_playlist_time->status = 0;
        node_save($broadcast_playlist_time);
        $success = true;
    }

    echo json_encode(array('success' => $success));

    module_invoke_all('exit');
    exit;


}
function broadcast_remove_playlist_ajax(){
    global $user;
    $broadcast_id = isset($_POST['broadcast_id']) ? $_POST['broadcast_id'] : 0;
    $playlist_id = isset($_POST['playlist_id']) ? $_POST['playlist_id'] : 0;
    if($broadcast_id && $playlist_id){
        $query = db_select('node','n');
        $query->fields('n',array('nid'));
        $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
        $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
        $query->leftJoin('field_data_field_playlist_broadcast_time_id','pbt','pbt.entity_id=n.nid');
        $query->condition('field_playlist_broadcast_time_id_nid',$playlist_id);
        $result = $query->execute()->fetchAssoc();
        if($result['nid']){
            node_delete($result['nid']);
            $success = true;
        }else{
            $success = false;
        }
    }

    echo json_encode(array('success' => $success));

    module_invoke_all('exit');
    exit;


}

