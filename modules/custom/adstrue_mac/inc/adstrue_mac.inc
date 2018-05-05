<?php
// Form Update MAC
function adstrue_mac_update_property($form,&$form_state,$value) {

    drupal_add_css(drupal_get_path('module','adstrue_mac').'/assets/css/mac.css');

    drupal_add_js(drupal_get_path('module','adstrue_mac').'/assets/js/autocomplete_selected.js');

    $form = bulid_form_mac($value,$form_state);
    return $form;
}

/*
* From tạo mới thiết bị
*/
function adstrue_mac_add_property($form,$form_state) {
    drupal_add_css(drupal_get_path('module','adstrue_mac').'/assets/css/mac.css');

    drupal_add_js(drupal_get_path('module','adstrue_mac').'/assets/js/autocomplete_selected.js');
    
    return bulid_form_mac(0,$form_state);
}

/* 
* From tạo mới MAC
*/
function bulid_form_mac($tid = 0,$form_state = array()) {
    $term = taxonomy_term_load($tid);
    // Lấy MAc vocabulary id 
    $mVoc = taxonomy_vocabulary_machine_name_load('mac_address');
    $mVid = $mVoc->vid;
    // Kiểm tra xem có phải là thuộc MAC taxonomy không
    if($term) {
        if($term->vid != $mVid) {
            drupal_not_found();
        }
    }

    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;

    // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }

    if(isset($_GET['company']) && (int)$_GET['company'] > 0) {
       $company_id = $_GET['company'];
       $rgSe  = filter_region_by_company($company_id);
    }
    if(isset($_GET['region']) && (int)$_GET['region'] > 0) {
       $region_id = $_GET['region'];
       $prSe = filter_province_by_region($region_id);
    }

    if(isset($_GET['province']) && (int)$_GET['province'] > 0) {
        $province_id = $_GET['province'];
        $stSe = filter_store_by_province($province_id);
    }
    if(isset($_GET['store']) && (int)$_GET['store'] > 0) {
        $store_id = $_GET['store'];
    }

    if(isset($term->field_mac_company_id['und'][0]['tid']) && !empty($term->field_mac_company_id['und'][0]['tid'])) {
        $company_id = $term->field_mac_company_id['und'][0]['tid'];
        $rgSe = filter_region_by_company($company_id);
     }
     if(isset($term->field_mac_region_id['und'][0]['tid']) && !empty($term->field_mac_region_id['und'][0]['tid'])) {
        $region_id = $term->field_mac_region_id['und'][0]['tid'];
        $prSe = filter_province_by_region($region_id);
     }
     if(isset($term->field_mac_province_id['und'][0]['tid']) && !empty($term->field_mac_province_id['und'][0]['tid'])) {
        $province_id = $term->field_mac_province_id['und'][0]['tid'];
        $stSe = filter_store_by_province($province_id);

     }
     $mac_sap = '';
     if(isset($term->field_mac_store_id['und'][0]['tid']) && !empty($term->field_mac_store_id['und'][0]['tid'])) {
        $store_id = $term->field_mac_store_id['und'][0]['tid'];
        $store = taxonomy_term_load($store_id);
        if(isset($store->field_store_sap['und'][0]['value'])) {
            $mac_sap = $store->field_store_sap['und'][0]['value'];
        }
     }

    if(isset($term->field_device_status['und'][0]['value']) && !empty($term->field_device_status['und'][0]['value'])) {
        $status_id = $term->field_device_status['und'][0]['value'];
    }
    if(isset($term->field_mac_ip['und'][0]['value']) && !empty($term->field_mac_ip['und'][0]['value'])) {
         $mac_ip = $term->field_mac_ip['und'][0]['value'];
     }
    if(isset($term->field_device_status['und'][0]['value']) && !empty($term->field_device_status['und'][0]['value'])) {
          $trangthai = $term->field_device_status['und'][0]['value'];
    }

    $public_internet = isset($term->field_public_internet['und'][0]['value']) ? $term->field_public_internet['und'][0]['value'] : 1;

    // Xay dung form
    $form['mac_term'] = array(
       '#type' => 'textfield', 
       '#title' => t('Địa chỉ Mac'), 
       '#maxlength' => 60, 
       '#default_value' => (isset($term) && $term) ? $term->name : '',
       '#autocomplete_path' =>'mac/search_path',
       '#required' => TRUE,
    );
    $form['mac_tid'] = array(
       '#type' => 'hidden', 
       '#title' => t('Tìm địa chỉ Mac'), 
       '#value' => (isset($tid)) ? $tid : 0,
    );
    $form['mac_ip'] = array(
       '#type' => 'textfield', 
       '#title' => t('MAC IP'), 
       '#maxlength' => 60,
       '#default_value' => (isset($mac_ip)) ? $mac_ip :'',
    );
    $form['mac_trangthai'] = array(
       '#type' => 'select', 
       '#title' => t('Trạng thái'),
       '#options' => array(1=>'Đã kích hoạt', 2=>'Đã khóa'),
       '#default_value' => (isset($trangthai)) ? $trangthai : '',
    );
    $form['company_id'] = array(
      '#type' => 'select',
      '#title' => t('Tên công ty'),
      '#options' => $cSelect,
      '#ajax' => array(
        'callback' => 'mac_load_region_by_company',
        'method' =>'replace',
        'wrapper' => 'region-wrapper',
      ),
      '#default_value' => (isset($company_id)) ? $company_id : 0,
    );
    $form['region_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div id="region-wrapper">',
        '#title' => t('Chọn khu vực'),
        '#options' => (isset($rgSe) && $rgSe) ? $rgSe : array(0 => t('Chọn')),
        '#ajax' => array(
          'callback' => 'mac_load_province_by_region',
          'method' =>  'replace',
          'wrapper' => 'province-wrapper',
        ),
        '#default_value' => (isset($region_id)) ? $region_id : 0,
        '#suffix' => '</div>',
        '#validated' => TRUE
    );
    $form['province_id'] = array(
      '#type' => 'select',
      '#prefix' => '<div id="province-wrapper">',
      '#title' => t('Chọn tỉnh thành'),
      '#ajax' => array(
        'callback' => 'mac_load_store_by_province',
        'method' =>  'replace',
        'wrapper' => 'replace_autocomplete_field',
      ),
      '#options' =>  (isset($prSe) && $prSe) ? $prSe : array(0 => t('Chọn')),
      '#default_value' =>(isset($province_id)) ? $province_id  : 0,
      '#suffix' => '</div>',
      '#validated' => TRUE
    );

    $form['autocomplete_store_string_container'] = array(
        '#type' => 'container',
        '#tree' => TRUE,
        '#prefix' => '<div id="replace_autocomplete_field">',
        '#suffix' => '</div>',
    );
    // Lay gia tri cho tryen vao autocomplete textfield
    $query_parameter = isset($province_id) ? $province_id : 0;
    if(isset($form_state['values']['province_id'])) {
        $query_parameter = $form_state['values']['province_id'];
    }

    $masap_value_autocomplete = ($mac_sap) ? $mac_sap.'|_'.$store->name.'|_'.$store->tid : '';

    $form['autocomplete_store_string_container']['store_string'] = array(
         '#type' => 'textfield',
         '#attributes' => array(
            'placeholder' => 'Nhập mã SAP cửa hàng',
         ),
         '#autocomplete_path' => 'mac/search_sap/'.$query_parameter,
         '#title' => t('Chọn cửa hàng'),
         '#maxlength' => 120,
         '#default_value' => $masap_value_autocomplete,
    );
    $form['public_internet'] = array(
        '#type' => 'select',
        '#title' => t('Public Internet'),
        '#options' => array(1 => 'Local', 2 => 'Public Internet'),
        '#default_value' => $public_internet
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#weight' => 6,
        '#value' => t('Cập nhật')
    );
    return $form;
}

function adstrue_mac_delete($form,&$form_state,$value) {
    global $base_url;
    $voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $vid = $voca->vid;

    $term = taxonomy_term_load($value);
    if($term->vid !== $vid) {
        drupal_not_found('Không thể tìm thấy thiết bị');
    }
    if($term){
        $form['mac_address'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
            ),
            '#prefix' => '<div class="">',
            '#suffix' => '</div>'

        );
        $form['mac_address']['header'] = array(
            '#type' => 'item',
            '#markup' =>'<h3 class="col-md-10">Bạn có chắc chắn muốn xóa thiết bị ? </h3> <a href="'.$base_url.'/mac/list" class="btn btn-info pull-right" style="margin-top:24px;"><i class="fa fa-reply"></i> Quay lại </a>',
            '#prefix' => '<div class="box-header with-border">',
            '#suffix' => '</div>'
        );
        $form['mac_address']['body'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );
        $form['mac_address']['body']['name_cat'] = array(
            '#type' => 'item',
            '#markup' => '<b>Địa chỉ MAC : </b>' .$term->name,
            '#prefix' => '<div class="col-md-4">',
            '#suffix' => '</div>'
        );
        if(isset($term->field_mac_store_id['und'][0]['tid'])) {
            $store = taxonomy_term_load($term->field_mac_store_id['und'][0]['tid']);
            $form['mac_address']['body']['name_store'] = array(
                '#type' => 'item',
                '#markup' => '<b>Tên cửa hàng : </b>' .$store->name,
                '#prefix' => '<div class="col-md-6">',
                '#suffix' => '</div>'
            );
        }
        $form['mac_address']['body']['id_mac'] = array(
            '#type' => 'hidden',
            '#value' => $term->tid,
        );
        
        $form['mac_address']['footer'] = array(
            '#type' => 'container',
            '#prefix' => '<div class="box-footer width-border">',
            '#suffix' => '</div>'
        );
        $form['mac_address']['footer']['delete_slide'] = array(
            '#type' => 'submit',
            '#value' => 'Xóa thiết bị',
            '#attributes' => array(
                'class' => array('btn','btn-danger')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>'
        );

        return $form;
    }
}

function adstrue_mac_delete_submit($form,&$form_state) {
    $tid = $form_state['values']['id_mac'];
    $tax = taxonomy_term_load($tid);
    $voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $vid = $voca->vid;
    if($tax->vid == $vid) {
      taxonomy_term_delete($tid);
      drupal_set_message('Đã xóa thiết bị');
    } else {
      drupal_set_message('Không tìm thấy thiết bị','error');
    }
    $form_state['redirect'] = 'mac/list';
}
/*
* Form hien thi danh sach MAC
*/
function manage_mac_by_list($form, &$form_state) {
    $header = array(
        array('data' => 'STT'),
        array('data' => 'SAP'),
        array('data' => 'Cửa hàng'),
        array('data' => 'Tỉnh thành'),
        array('data' => 'IP'), 
        array('data' => 'Địa chỉ MAC'),
        array('data' => 'Phiên bản'),
        array('data' => 'Tình trạng'),
        array('data' => 'Trạng thái'),
        array('data' => 'Last Login'),
        array('data' => 'Nguồn phát'),
        array('data' => 'Thao tác'),
    );
    $form = array();
    $form += statistic_mac_block($form_state);
    $form += statistic_mac_view($header,$form_state,1);
    return $form;
}

function manage_mac_by_view($form, &$form_state) {
    $header = array(
        array('data' => 'STT'),
        array('data' => 'SAP'),
        array('data' => 'Cửa hàng'),
        array('data' => 'Tỉnh thành'),
        array('data' => 'IP'), 
        array('data' => 'Địa chỉ MAC'),
        array('data' => 'Phiên bản'),
        array('data' => 'Tình trạng'),
        array('data' => 'Trạng thái'),
        array('data' => 'Last Login'),
        array('data' => 'Resource'),
    );

    $form = array();

    $form += statistic_mac_block($form_state);

    $form += statistic_mac_view($header,$form_state,0);
    return $form;
}

function statistic_mac_view($header,$form_state,$action = 1) {
    drupal_add_css(drupal_get_path('module','adstrue_mac').'/assets/css/mac.css');
    global $user,$base_url;
    // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
        $cSelect[$company->tid] = $company->name;
    }
    $company_id = $region_id = $province_id = $store_id = $trangthai = $tinhtrang = $public_internet = 0;
    $macip = $mac_sap = $store_name = '';

    $values = isset($form_state['values']) ? $form_state['values'] : array();
    if(isset($_GET['get']) && $_GET['get'] == 1) {
        $values = $_GET;
    }

    if(isset($values['company']) && (int)$values['company'] > 0) {
        $company_id = $values['company'];
        $rgSe = filter_region_by_company($company_id);
    }

    if(isset($values['region']) && (int)$values['region'] > 0) {
        $region_id = $values['region'];
        $prSe  = filter_province_by_region($region_id);
    }
    if(isset($values['province']) && (int)$values['province'] > 0) {
        $province_id = $values['province'];
        $stSe = filter_store_by_province($province_id);
    }
    if(isset($values['store']) && (int)$values['store'] > 0) {
        $store_id = $values['store'];  
    }
    if(isset($values['trangthai']) && (int)$values['trangthai'] > 0) {
        $trangthai = $values['trangthai'];
    }
    if(isset($values['tinhtrang']) && (int)$values['tinhtrang'] > 0) {
        $tinhtrang = $values['tinhtrang'];
    }

    if(isset($values['public_internet']) && (int)$values['public_internet'] > 0) {
        $public_internet = $values['public_internet'];
    }

    if(isset($values['version_code']) && !empty($values['version_code'])) {
        $version_code = $values['version_code'];
    } else {
        $version_code = 0;
    }
    if(isset($values['keyword']) && !empty($values['keyword'])) {
        $keyword = $values['keyword'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    if(isset($values['macip']) && !empty($values['macip'])) {
        $macip = $values['macip'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    if(isset($values['mac_sap']) && !empty($values['mac_sap'])) {
        $mac_sap = $values['mac_sap'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    if(isset($values['store_name']) && !empty($values['store_name'])) {
        $store_name = $values['store_name'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }

    $rows = show_mac_adtrue_list($form_state,$action);

    $form['danhsach_mac'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['danhsach_mac']['header_filter'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>',
    );

    $form['danhsach_mac']['header_filter']['company'] = array(
         '#type' => 'select',
         '#title' => t('Công ty'),
         '#options' => $cSelect,
         '#ajax' => array(
            'callback' => 'mac_load_region_by_company_table',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
          ),
         '#default_value' => isset($company_id) ? $company_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item">',
         '#suffix' => '</div>',
    );
    $form['danhsach_mac']['header_filter']['region'] = array(
         '#type' => 'select',
         '#title' => t('Vùng miền'),
         '#options' => (isset($rgSe)) ? $rgSe : array(0 => t('Chọn vùng miền')),
         '#ajax' => array(
            'callback' => 'mac_load_province_by_region_table',
            'method' =>  'replace',
            'wrapper' => 'province-wrapper',
          ),
         '#default_value' => isset($region_id) ? $region_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="region-wrapper">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE,
    );
    $form['danhsach_mac']['header_filter']['province'] = array(
         '#type' => 'select',
         '#title' => t('Tỉnh thành'),
         '#options' => isset($prSe) ? $prSe : array(0 => t('Chọn tỉnh thành')),
         '#ajax' => array(
            'callback' => 'mac_load_store_by_province_table',
            'method' =>  'replace',
            'wrapper' => 'store-wrapper',
          ),
         '#default_value' => isset($province_id) ? $province_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="province-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE
    );
    $form['danhsach_mac']['header_filter']['store_name'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên cửa hàng hoặc mã SAP'),
        '#size' => 60,
        '#default_value' => isset($store_name) ? $store_name : NULL,
        '#attributes' => array(
           'placeholder' => 'Tên cửa hàng hoặc mã SAP',
        ),
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-3 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['danhsach_mac']['header_filter']['trangthai'] = array(
         '#type' => 'select',
         '#title' => t('Trạng thái'),
         '#options' => array(0=>'Tất cả',1 => 'Online',2=>'Offline'),
         '#prefix' => '<div class="col-md-2 box-search-item">',
         '#default_value' => isset($trangthai) ? $trangthai : 0,
         '#suffix' => '</div>',
         '#validated' => TRUE
    );
    $form['danhsach_mac']['header_filter']['macip'] = array(
        '#type' => 'textfield',
        '#title' => t('Địa chỉ IP'),
        '#size' => 60,
        '#attributes' => array(
          'placeholder' => 'Địa chỉ IP',
        ),
        '#default_value' => isset($macip) ? $macip : NULL,
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );

    $form['danhsach_mac']['header_filter']['keyword'] = array(
        '#type' => 'textfield',
        '#title' => t('Địa chỉ MAC'),
        '#size' => 60,
        '#default_value' => isset($keyword) ? $keyword : NULL,
        '#attributes' => array(
            'placeholder' => 'Địa chỉ MAC',
        ),
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );
    $public_internet_options = array(
        '0' => 'Resource',
        '1' => 'Local',
        '2' => 'Public'
    );
    $form['danhsach_mac']['header_filter']['public_internet'] = array(
        '#type' => 'select',
        '#title' => 'Nguồn phát',
        '#options' => $public_internet_options,
        '#default_value' => $public_internet,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );

    $version_options = array(
        '0' => 'Chọn phiên bản',
        '1' => '1',
        '2' => '2'
    );
    $form['danhsach_mac']['header_filter']['version_code'] = array(
        '#type' => 'select',
        '#title' => 'Phiên bản',
        '#options' => $version_options,
        '#default_value' => $version_code,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );
    $form['danhsach_mac']['header_filter']['tinhtrang'] = array(
         '#type' => 'select',
         '#title' => t('Tình trạng'),
         '#options' => array(0=>'Tất cả',1 => 'Đã kích hoạt',2=>'Đang khóa'),
         '#prefix' => '<div class="col-md-2 box-search-item">',
         '#default_value' => isset($tinhtrang) ? $tinhtrang : 0,
         '#suffix' => '</div>',
         '#validated' => TRUE
    );
    
    $form['danhsach_mac']['header_filter']['submit_search'] = array(
        '#type' => 'submit',
        '#weight' => 2,
        '#value' => t('Tìm kiếm'),
        '#prefix' => '<div class="col-md-6"></div><div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
        '#submit' => array('filter_mac_by_search'),
    );

    $form['danhsach_mac']['header_filter']['export_excel'] = array(
        '#type' => 'submit',
        '#weight' => 3,
        '#value' => 'Xuất excel',
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
        '#submit' => array('export_excel_submit')
    );

    $form['danhsach_mac']['header_filter']['create_mac'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/mac/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Thêm thiết bị </a>',
        '#weight' => 4,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );

    //body of table
    $values = isset($form_state['values']) ? $form_state['values'] : array();
    if(isset($_GET['get']) && $_GET['get'] == 1) {
        $values = $_GET;
    }
    
    $result = add_condition_search_mac($values,0,0);
    $total = 0;
    if($result) {
        $total = $result->rowCount();
    }
    $message = '<h4>Tổng số thiết bị : <b>'.$total.'</b></h4>';

    $form['danhsach_mac']['search_summary'] = array(
        '#type' => 'item',
        '#markup' => '<div class="callout callout-success">'.$message.'</div>',
        '#prefix' => '<div class="col-md-12" style="margin-top : 30px;">',
        '#suffix' => '</div>'
    );
    $form['danhsach_mac']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>',

    );
    $form['danhsach_mac']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}

function statistic_mac_block($form_state='') {
    $online = 0;
    $offline = 0;
    $dakhoa = 0;
    $dakichhoat = 0;

    $voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $vid = $voca->vid;

    $query = db_select('taxonomy_term_data','ttd');
    $query->fields('ttd',array('tid'));
    $query->fields('mll',array('field_mac_last_login_value'));
    $query->fields('ds',array('field_device_status_value'));
    $query->condition('ttd.vid',$vid);
    $query->leftJoin('field_data_field_mac_last_login','mll','mll.entity_id=ttd.tid');
    $query->leftJoin('field_data_field_device_status','ds','ds.entity_id=ttd.tid');

    $result = $query->execute();

    $tongso = $result->rowCount();
    if($tongso) {
        foreach ($result as $row) {
            $last_login = isset($row->field_mac_last_login_value) ? $row->field_mac_last_login_value : 0;
            if($last_login) {
                $curTime = time();
                // Thoi gian sau last login 15 phút
                $onLog = (int)$last_login + 15*60;

                if($curTime > $onLog) {
                    $offline++;
                } else {
                    $online++;
                }
            }

            $trangthai = isset($row->field_device_status_value) ? $row->field_device_status_value : '';
            if($trangthai == 1) {
                $dakichhoat++;
            }
            if($trangthai == 2) {
                $dakhoa++;
            }
        }

    }
    $str_online = '<div class="info-box">
                <span class="info-box-icon bg-green"><i class="fa fa-tv"></i></span>
                <div class="info-box-content">
                    <span class="info-box-text">'.t('Online').'</span>
                    <span class="info-box-number">'.$online.'</span>
                </div>            
            </div>';
    $str_offline = '<div class="info-box">
                <span class="info-box-icon bg-red"><i class="fa fa-warning"></i></span>
                <div class="info-box-content">
                    <span class="info-box-text">'.t('Offline').'</span>
                    <span class="info-box-number">'.$offline.'</span>
                </div>            
            </div>';
    $str_active_device = '<div class="info-box">
                <span class="info-box-icon bg-aqua"><i class="fa  fa-check-circle-o"></i></span>
                <div class="info-box-content">
                    <span class="info-box-text">'.t('Đã kích hoạt').'</span>
                    <span class="info-box-number">'.$dakichhoat.'</span>
                </div>            
            </div>';
    $str_deactive_device = '<div class="info-box">
                <span class="info-box-icon bg-yellow"><i class="fa fa-database"></i></span>
                <div class="info-box-content">
                    <span class="info-box-text">'.t('Tổng số thiết bị').'</span>
                    <span class="info-box-number">'.$tongso.'</span>
                </div>            
            </div>';
    $form['summary_block'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['online'] = array(
        '#type' => 'item',
        '#markup' => $str_online,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['offline'] = array(
        '#type' => 'item',
        '#markup' => $str_offline,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['active'] = array(
        '#type' => 'item',
        '#markup' => $str_active_device,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['deactive'] = array(
        '#type' => 'item',
        '#markup' => $str_deactive_device,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );

    return $form;

}
/*
* Export excel
*/
function export_excel_submit($form,$form_state) {
    $path_class = drupal_get_path('module','adstrue_mac') .'/export_data_excel.php';
    include $path_class;
    $values = $form_state['values'];
    $result = add_condition_search_mac($values,0,0);

    $exporter = new ExportDataExcel('browser', 'mac_list.xls');
    $exporter->initialize();
    $exporter->addRow(array('STT','SAP','Tên Cửa hàng','TỈnh thành','IP','MAC','Phiên bản','Tình trạng','Trạng thái','Last Login','Resource','Thời gian Offline'));

    if($result) {
        if($result->rowCount()) {
              $i = 1;
              foreach($result as $row) {
                    $term = taxonomy_term_load($row->tid);
                    
                    $store_id = (isset($term->field_mac_store_id['und'][0]['tid'])) ? $term->field_mac_store_id['und'][0]['tid'] : 0;
                    $store = taxonomy_term_load($store_id);
                    $mac_sap = (isset($store->field_store_sap['und'][0]['value'])) ? $store->field_store_sap['und'][0]['value'] : '';
                    
                    $province_id = (isset($term->field_mac_province_id['und'][0]['tid'])) ? $term->field_mac_province_id['und'][0]['tid'] : 0;

                    $province = taxonomy_term_load($province_id);

                    $region_id = (isset($term->field_mac_region_id['und'][0]['tid'])) ? $term->field_mac_region_id['und'][0]['tid'] : 0;
                    $region = taxonomy_term_load($region_id);

                    $company_id = isset($term->field_mac_company_id['und'][0]['tid']) ? $term->field_mac_company_id['und'][0]['tid'] : 0;
                    $company = taxonomy_term_load($company_id);
                    
                    $mac_ip = (isset($term->field_mac_ip['und'][0]['value'])) ? $term->field_mac_ip['und'][0]['value'] : '';

                    $last_login = isset($term->field_mac_last_login['und'][0]['value']) ? $term->field_mac_last_login['und'][0]['value'] : null;

                    $frendlyTime = '';
                    if($last_login) {
                        $curTime = time();
                        $config = adstrue_mac_config();

                        // Thoi gian sau last login 15 phút
                        $onLog = (int)$last_login + $config['status_time']*60;

                        if($curTime > $onLog) {
                            $status = 'Offline';
                            $frendlyTime = secondsToFrendlyTime($last_login);
                        } else {
                           $status= 'Online';
                        }
                    } else {
                        $status = '';
                    }
                    //

                    // 
                    $trangthai = (isset($term->field_device_status['und'][0]['value'])) ? $term->field_device_status['und'][0]['value'] : '';
                    if($trangthai == 1) {
                       $matt = 'Đã kích hoạt';
                    } 
                    if($trangthai == 2) {
                       $matt = 'Đã khóa';
                    }

                    $version_code = '';
                    if(isset($term->field_launcher_version['und'][0]['value'])) {
                        $version_code = $term->field_launcher_version['und'][0]['value'];
                    }

                    $public_internet_value = isset($term->field_public_internet['und'][0]['value']) ? $term->field_public_internet['und'][0]['value'] : 1;
                    $public_internet = ($public_internet_value == 2) ? 'Public' : 'Local';

                    $row = array(
                        $i,
                        $mac_sap,
                        isset($store->name) ? $store->name : '',
                        isset($province->name) ? $province->name : '',
                        $mac_ip,
                        $term->name,
                        $version_code,
                        $matt,
                        $status,
                        (isset($last_login)) ? date('H:i:s d/m/Y', $last_login) : '',
                        $public_internet,
                        $frendlyTime
                    );

                    $exporter->addRow($row);
                    $i++;
              }
            }
    }
    $exporter->finalize();
    exit();
}

function filter_mac_by_search($form,&$form_state) {
    global $base_url;
    $values  = $form_state['values'];
    $get = '?get=1';
    foreach($values as $key => $val) {
        if($key !== 'submit_search' && $key !== 'export_excel' && $key !== 'form_build_id' && $key !== 'form_token' && $key !== 'form_id' && $key !== 'op') {
            $get .= '&'.$key.'='.$val;
        }
    }
    $form_state['redirect'] = $base_url.'/mac/list'.$get;
}


function show_mac_adtrue_list($form_state = array(),$action=1) {

    global $user;
    global $base_url;
    $limit = 50;
    $values = isset($form_state['values']) ? $form_state['values'] : array();
    if(isset($_GET['get']) && $_GET['get'] == 1) {
        $values = $_GET;
    }

    $result = add_condition_search_mac($values,1,$limit);
    if(isset($_GET['page']) && (int)$_GET['page'] > 0) {
        $page  = (int)$_GET['page'];
        $i = $limit*$page + 1;
    } else {
        $i = 1;
    }

    $rows = array();
    if($result) {
        if($result->rowCount()) {
            foreach($result as $row) {
                $term = taxonomy_term_load($row->tid);
                $store_id = (isset($term->field_mac_store_id['und'][0]['tid'])) ? $term->field_mac_store_id['und'][0]['tid'] : 0;
                $store = taxonomy_term_load($store_id);
                $mac_sap = (isset($store->field_store_sap['und'][0]['value'])) ? $store->field_store_sap['und'][0]['value'] : '';
                
                $province_id = (isset($term->field_mac_province_id['und'][0]['tid'])) ? $term->field_mac_province_id['und'][0]['tid'] : 0;
                $province = taxonomy_term_load($province_id);

                $region_id = (isset($term->field_mac_region_id['und'][0]['tid'])) ? $term->field_mac_region_id['und'][0]['tid'] : 0;
                $region = taxonomy_term_load($region_id);
                
                $mac_ip = (isset($term->field_mac_ip['und'][0]['value'])) ? $term->field_mac_ip['und'][0]['value'] : '';

                $last_login = isset($term->field_mac_last_login['und'][0]['value']) ? $term->field_mac_last_login['und'][0]['value'] : null;
                if($last_login) {
                    $curTime = time();
                    $config = adstrue_mac_config();
                    // Thoi gian sau last login 15 phút
                    $onLog = (int)$last_login + ($config['status_time']*60);

                    if($curTime > $onLog) {
                        $off_time = $curTime - $last_login;
                        $frendlyTime = secondsToFrendlyTime($last_login);
                        $status = '<p style="width: 110px;"><span class="label label-warning">Offline - '.$frendlyTime.'</span></p>';
                        if($off_time > 86400) {
                            $status = '<p style="width: 110px;"><span class="label label-danger">Offline - '.$frendlyTime.'</span></p>';
                        }
                    } else {
                        $status= '<span class="label label-success">Online</span>';
                    }
                } else {
                    $status = '';
                }

                $trangthai = (isset($term->field_device_status['und'][0]['value'])) ? $term->field_device_status['und'][0]['value'] : '';
                if($trangthai == 1) {
                   $matt = '<span class="label label-success">Đã kích hoạt</span>';
                } 
                if($trangthai == 2) {
                   $matt = '<span class="label label-warning">Đã khóa</span>';
                }

                $version_code = isset($term->field_launcher_version['und'][0]['value']) ? $term->field_launcher_version['und'][0]['value'] :1;


                $public_internet_value = isset($term->field_public_internet['und'][0]['value']) ? $term->field_public_internet['und'][0]['value'] : 1;
                $public_internet = ($public_internet_value == 2) ? 'Public' : 'Local';

                if($action) {
                    $rows[] = array(
                        $i,
                        '<a href="'.$base_url.'/store?ma_sap='.$mac_sap.'" target="_blank" title="">'.$mac_sap.'</a>',
                        isset($store->name) ? $store->name : '',
                        isset($province->name) ? $province->name : '',
                        $mac_ip,
                        '<a href="'.$base_url.'/mac/check/history/'.$term->tid.'">'.$term->name.'</a>',
                        '<span class="badge bg-aqua">'.$version_code.'</span>',
                        $matt,
                        $status,
                        (isset($last_login)) ? date('H:i:s d/m/Y', $last_login) : '',
                        $public_internet,
                        '<div class="btn-group">
                              <button type="button" class="btn btn-success">Thao tác</button>
                              <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                                  <span class="caret"></span>
                                  <span class="sr-only">Toggle Dropdown</span>
                              </button>
                              <ul class="dropdown-menu" role="menu">
                                  <li><a href="'.$base_url.'/mac/check/history/'.$term->tid.'" ><i class="fa fa-files-o"></i> Nhật kí</a></li>
                                  <li><a href="'.$base_url.'/mac/edit/'.$term->tid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                                  <li><a href="'.$base_url.'/mac/delete/'.$term->tid.'" ><i class="fa fa-trash"></i> Xóa</a></li>
                              </ul>
                          </div>'
                    );
                }else{
                    $rows[] = array(
                        $i,
                        '<a href="'.$base_url.'/store?ma_sap='.$mac_sap.'" target="_blank" title="">'.$mac_sap.'</a>',
                        isset($store->name) ? $store->name : '',
                        isset($province->name) ? $province->name : '',
                        $mac_ip,
                        $term->name,
                        $version_code,
                        $matt,
                        $status,
                        (isset($last_login)) ? date('H:i:s d/m/Y', $last_login) : '',
                    );
                }
                $i++;
            }
        }
    }
    return $rows;
}

/* 
* handle the submit action to update mac 
*/
function adstrue_mac_update_property_submit($form,&$form_state) {
    // Lay ten dia chi mac
    $nMac = $form_state['values']['mac_term'];
    $public_internet = $form_state['values']['public_internet'];
    $company_id = $form_state['values']['company_id'];
    if($company_id == 0) {
      $company_id = null;
    }
    $region_id = $form_state['values']['region_id'];
    if($region_id == 0) { 
      $region_id = null;
    }
    $province_id = $form_state['values']['province_id'];
    if($province_id == 0) {
       $province_id = null;
    }
    $store_str = $form_state['values']['autocomplete_store_string_container']['store_string'];

    if($store_str !== '') {
        $store_str_arr = explode('|_', $store_str);
        $store_id = end($store_str_arr);
        if(count($store_str_arr) > 3) {
            drupal_set_message('Tên cửa hàng không hợp lệ','error');
            $form_state['rebuild'] = TRUE;

        }
    }
    
    $trangthai = $form_state['values']['mac_trangthai'];

    $mac_ip = $form_state['values']['mac_ip'];

    $mac_tid = $form_state['values']['mac_tid'];
    $mac_name = $form_state['values']['mac_term'];

    $nMac = trim($nMac);

    // Lay ra dia chi mac tu db
    $result = db_select('taxonomy_term_data','ttd')
          ->fields('ttd')
          ->condition('name', $nMac,'=')
          ->execute()
          ->fetchAll();
    
    // Kiem tra xem mac co ton tai hay khong 
    if($result) {
        foreach($result as $mac) {
          $mac->field_mac_ip['und'][0]['value'] = $mac_ip;
          $mac->field_mac_company_id['und'][0]['tid'] = $company_id;
          $mac->field_mac_region_id['und'][0]['tid'] = $region_id;
          $mac->field_mac_province_id['und'][0]['tid'] = $province_id;
          $mac->field_mac_store_id['und'][0]['tid'] = $store_id;
          $mac->field_device_status['und'][0]['value'] = $trangthai;
          $mac->field_public_internet['und'][0]['value'] = $public_internet;
          $status = taxonomy_term_save($mac);

          if($status == 2) {
            drupal_set_message('Cập nhật thành công thiết bị');
            $form_state['redirect'] = 'mac/list';
          }
        }
      } elseif((int)$mac_tid > 0) {
        $mac = taxonomy_term_load($mac_tid);
        $mac->name = $mac_name;
        $mac->field_mac_ip['und'][0]['value'] = $mac_ip;
        $mac->field_mac_company_id['und'][0]['tid'] = $company_id;
        $mac->field_mac_region_id['und'][0]['tid'] = $region_id;
        $mac->field_mac_province_id['und'][0]['tid'] = $province_id;
        $mac->field_mac_store_id['und'][0]['tid'] = $store_id;
        $mac->field_device_status['und'][0]['value'] = $trangthai;
        $mac->field_public_internet['und'][0]['value'] = $public_internet;
        $status = taxonomy_term_save($mac);

        if($status == 2) {
            drupal_set_message('Cập nhật thành công thiết bị');
            $form_state['redirect'] = 'mac/list';
            return;
        } else {
            drupal_set_message('Không thể cập nhật thiết bị');
            $form_state['redirect'] = 'mac/list';
            return;
        }

      } else {
           $form_state['redirect'] = 'mac/list';
           drupal_set_message('Không thể cập nhật thiết bị');
           return;
      }
}

// Submit to create mac
function adstrue_mac_add_property_submit($form,&$form_state) {
    global $user;
    // Lay ten dia chi mac
    $nMac = $form_state['values']['mac_term'];
    $public_internet = $form_state['values']['public_internet'];
    $company_id = $form_state['values']['company_id'];
    if($company_id == 0) {
      $company_id = null;
    }
    $region_id = $form_state['values']['region_id'];
    if($region_id == 0) { 
      $region_id = null;
    }
    $province_id = $form_state['values']['province_id'];
    if($province_id == 0) {
       $province_id = null;
    }
    $store_str = $form_state['values']['autocomplete_store_string_container']['store_string'];
    if($store_str !== '') {
        $store_str_arr = explode('|_', $store_str);
        $store_id = end($store_str_arr);
    }

    $trangthai = $form_state['values']['mac_trangthai'];

    $mac_ip = $form_state['values']['mac_ip'];

    $mac_tid = $form_state['values']['mac_tid'];
    $mac_name = $form_state['values']['mac_term'];

    $nMac = trim($nMac);

    $voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $vid = $voca->vid;
    $tax = new stdClass();
    $tax->name = $nMac;
    $tax->vid = $vid;
    $tax->language = LANGUAGE_NONE;
    $tax->field_mac_company_id['und'][0]['tid'] = $company_id;
    $tax->field_mac_region_id['und'][0]['tid'] = $region_id;
    $tax->field_mac_province_id['und'][0]['tid'] = $province_id;
    $tax->field_mac_store_id['und'][0]['tid'] = $store_id;
    $tax->field_mac_ip['und'][0]['value'] = $mac_ip;
    $tax->field_device_status['und'][0]['value'] = $trangthai;
    $tax->field_public_internet['und'][0]['value'] = $public_internet;


    $status = taxonomy_term_save($tax);
    if($tax->tid) {
        drupal_set_message('Đã tạo mới 1 thiết bị');
        $form_state['redirect'] = 'mac/list';
    } else {
        drupal_set_message('Không thể cập nhật thiết bị','error');
        $form_state['redirect'] = 'mac/list';
    }
}

// update ngay gan thiet bi
function mac_update_created() {
    global $user;

    $mac_voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $mac_taxs = taxonomy_term_load_multiple(array(),array('vid' => $mac_voca->vid) );

    // Get the unix time of 22-10-2017
    $dateobj = DateTime::createFromFormat('d-m-Y', '22-10-2017');
    $unix_date = $dateobj->getTimestamp();

    foreach($mac_taxs as $mac) {
        if(!isset($mac->field_date_created_at['und'][0]['value']) ) {
            $mac->field_date_created_at['und'][0]['value'] = $unix_date;
            $mac->field_mac_person_add['und'][0]['uid'] = $user->uid;
        }
        taxonomy_term_save($mac);
    }
}


function secondsToFrendlyTime($seconds) {
    $dbDate = $seconds;
    $endDate = time();
    $diff = $endDate - $dbDate;
    $days = floor($diff/86400);
    $hours = floor(($diff-$days*86400)/(60 * 60));
    $min = floor(($diff-($days*86400+$hours*3600))/60);
    $second = $diff - ($days*86400+$hours*3600+$min*60);

    if($days > 0) return $days." ngày";
    elseif($hours > 0) return $hours." giờ";
    elseif($min > 0) return $min." phút";
    else return " ";
}

