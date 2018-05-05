<?php
/* 
* Build form to add add store
*/
function adstrue_vendor_add_store($form,&$form_state) {
	return build_form_store();
}
function adstrue_vendor_update_store($form,&$form_state,$value) {
	return build_form_store($value);
}

function build_form_store($tid = 0) {
	  drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');

	  $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;

    // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }

    $term = taxonomy_term_load($tid);
    // No phai thuoc vocabulary la cua hang moi duoc hien thi tren form
    $voca = taxonomy_vocabulary_machine_name_load('store');
    $vid = $voca->vid;
    if($term){
      if($term->vid !== $vid) {
        drupal_not_found();
      }
    }

    if(isset($term->field_company_store_id) && !empty($term->field_company_store_id)) {
	      $company_id = $term->field_company_store_id['und'][0]['tid'];
	      $rgSe = filter_region_by_company($company_id);
    }
    if(isset($term->field_region_store_id) && !empty($term->field_region_store_id)) {
        $region_id = $term->field_region_store_id['und'][0]['tid'];
        $prSe = filter_province_by_region($region_id);
    }
    if(isset($term->field_province_store_id) && !empty($term->field_province_store_id)) {
        $province_id = $term->field_province_store_id['und'][0]['tid'];
    }
    if(isset($term->field_store_sap) && !empty($term->field_store_sap)) {
        $sap = $term->field_store_sap['und'][0]['value'];
    }
    $store_description = (isset($term->description)) ? $term->description : '';
    $name_person = isset($term->field_store_contact_person['und'][0]['value']) ? $term->field_store_contact_person['und'][0]['value'] : '';
    $contact_number = isset($term->field_store_contact_number['und'][0]['value']) ? $term->field_store_contact_number['und'][0]['value'] : '';
    $contact_mobile = isset($term->field_store_contact_mobile['und'][0]['value']) ? $term->field_store_contact_mobile['und'][0]['value'] : '';
    $store_email = isset($term->field_store_email['und'][0]['value']) ? $term->field_store_email['und'][0]['value'] : '';
    $store_note = isset($term->field_store_noting['und'][0]['value']) ? $term->field_store_noting['und'][0]['value'] : '';

  	$form['fieldset_all'] = array(
        '#type' => 'container',
        '#weight' => 3, 
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Thông tin chung</h3>'
    );
    $form['fieldset_all']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );

    $form['fieldset_all']['body']['hidden_tid'] = array(
        '#type' => 'hidden',
        '#title' => t('Tên cửa hàng'),
        '#default_value' => (isset($term) && $term) ? $term->tid : '',
    );

    // Ten cửa hàng
    $form['fieldset_all']['body']['field_init'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr col-md-4">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['body']['field_init']['name_store'] = array(
      '#type' => 'textfield',
      '#title' => t('Tên cửa hàng'),
      '#default_value' => (isset($term) && $term) ? $term->name : '',
      '#required' => TRUE,
    );
    $form['fieldset_all']['body']['field_init']['store_masap'] = array(
      '#type' => 'textfield',
      '#title' => t('Mã SAP'),
      '#default_value' => (isset($sap)) ? $sap : '',
      '#required' => TRUE,
    );
    $form['fieldset_all']['body']['field_init']['name_person'] = array(
      '#type' => 'textfield',
      '#title' => t('Cửa hàng trưởng'),
      '#default_value' => $name_person,
    );
    $form['fieldset_all']['body']['field_init']['store_description'] = array(
        '#type' => 'textarea',
        '#title' => t('Địa chỉ'),
        '#default_value' => $store_description,
    );
    $form['fieldset_all']['body']['fieldset_property'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr col-md-4">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['body']['fieldset_property']['store_email'] = array(
        '#type' => 'textfield',
        '#title' => t('Email cửa hàng'),
        '#default_value' => (isset($store_email)) ? $store_email : '',
    );
    
    $form['fieldset_all']['body']['fieldset_property']['contact_number'] = array(
        '#type' => 'textfield',
        '#title' => t('Số điện thoại bàn'),
        '#default_value' => $contact_number,
    );
    $form['fieldset_all']['body']['fieldset_property']['contact_mobile'] = array(
        '#type' => 'textfield',
        '#title' => t('Số điện thoại cửa hàng trưởng'),
        '#default_value' => $contact_mobile,
    );
    $form['fieldset_all']['body']['fieldset_property']['store_note'] = array(
        '#type' => 'textarea',
        '#title' => t('Ghi chú'),
        '#default_value' => $store_note,
    );
    $form['fieldset_all']['body']['field_info'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr col-md-4">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['body']['field_info']['company_id'] = array(
        '#type' => 'select',
        '#title' => t('Tên công ty'),
        '#options' => $cSelect,
        '#ajax' => array(
            'callback' => 'match_region_by_company_store',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
        ),
        '#default_value' =>  (isset($company_id)) ? $company_id : 0,
        '#description' => t('Chọn công ty quản lí vùng miền'),
        '#required' => TRUE,
    );
    $form['fieldset_all']['body']['field_info']['region_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div id="region-wrapper">',
        '#title' => t('Chọn vùng miền'),
        '#options' => (isset($rgSe)) ? $rgSe : array(0 => t('Chọn')),
        '#ajax' => array(
            'callback' => 'match_province_by_region_store',
            'method' =>  'replace',
            'wrapper' => 'province-wrapper',
        ),
        '#default_value' => (isset($region_id)) ? $region_id : 0,
        '#suffix' => '</div>',
        '#description' => t('Chọn vùng miền'),
        '#validated' => TRUE,
    );
    $form['fieldset_all']['body']['field_info']['province_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div id="province-wrapper">',
        '#title' => t('Chọn tỉnh thành'),
        '#options' =>  (isset($prSe)) ? $prSe : array(0 => t('Chọn')),
        '#default_value' => (isset($province_id)) ? $province_id : 0,
        '#suffix' => '</div>',
        '#description' => t('Chọn tỉnh thành'),
        '#validated' => TRUE,
    );

    
    $form['fieldset_all']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );

    $form['fieldset_all']['footer']['fieldset_property']['submit'] = array(
        '#type' => 'submit',
        '#weight' => 6,
        '#value' => t('Lưu lại'),
        '#prefix' => '<div class="pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-primary')
        ),
    );
    $form['#validate'][] = 'adstrue_vendor_store_validate';
    $form['#submit'][] = 'adstrue_vendor_store_submit';
    return  $form;
}

function adstrue_vendor_import_store($form,&$form_state) {
    global $base_url;
    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;

    // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }
    $form = array();
    $form['upload_store'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['upload_store']['header'] = array(
        '#type' => 'container',
        '#weight' => 1,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['upload_store']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );

    $form['upload_store']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Import cửa hàng lên hệ thống</h3>'
    );
    $form['upload_store']['header']['header_filter'] = array(
        '#type' => 'container',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );
    $form['upload_store']['header']['header_filter']['company_id'] = array(
        '#type' => 'select',
        '#title' => t('Tên công ty'),
        '#options' => $cSelect,
        '#ajax' => array(
            'callback' => 'store_import_load_region_by_company',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
        ),
        '#prefix' => '<div class="col-md-3 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['upload_store']['header']['header_filter']['region_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div class="col-md-3 box-search-item"><div id="region-wrapper">',
        '#title' => t('Chọn khu vực'),
        '#options' => array(0 => t('Chọn')),
        '#ajax' => array(
          'callback' => 'store_import_load_province_by_region',
          'method' =>  'replace',
          'wrapper' => 'province-wrapper',
        ),
        '#suffix' => '</div></div>',
        '#validated' => TRUE
    );
    $form['upload_store']['header']['header_filter']['province_id'] = array(
        '#type' => 'select',
         '#title' => t('Tỉnh thành'),
         '#options' => array(0 => t('Chọn tỉnh thành')),
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="province-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE
    );
    $form['upload_store']['body']['file_store_to_upload'] = array(
        '#title' => t('Tải file excel'),
        '#type' => 'managed_file',
        '#upload_location' => 'public://import_store_file/',
        '#upload_validators' => array(
            'file_validate_extensions' => array('xlsx xls'),
        ),
        '#required' => TRUE,
    );
    $form['upload_store']['body']['demo_file'] = array(
        '#type' => 'item',
        '#markup' =>'<p><span>Download file excel mẫu: </span><a href="'.$base_url.'/sites/default/files/import_store_file/store_import.xlsx">Demo import store</a></p>',
    );
    $form['upload_store']['footer'] = array(
        '#type' => 'container',
        '#weight' => 4,
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );
    $form['upload_store']['footer']['submit'] = array(
        '#type' => 'submit',
        '#weight' => 2,
        '#value' => t('Load file'),
        '#attributes' => array(
            'class' => array('btn','btn-primary')
        ),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>',
    );
    return $form;
}

/*
* Delete store taxonomy form
*/
function delete_adstrue_vendor_store($form,&$form_state,$value) {
    $form = array();
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => '<h4>Vui lòng xác nhận để xóa cửa hàng</h4>',
    );
    $form['store_id'] = array(
        '#type' => 'hidden',
        '#value' => $value,
    );
    return confirm_form($form,'Vui lòng xác nhận để xóa cửa hàng','store',t(''),t('Xác nhận'),t('Hủy bỏ'));
}

function delete_adstrue_vendor_store_submit($form,&$form_state) {
    $tid = $form_state['values']['store_id'];
    $tax = taxonomy_term_load($tid);

    $voca = taxonomy_vocabulary_machine_name_load('store');
    $vid = $voca->vid;
    if($tax->vid == $vid && $tax) {
      taxonomy_term_delete($tid);
      drupal_set_message('Đã xóa cửa hàng');
      $form_state['redirect'] = 'store';
    } else {
      drupal_set_message('Không tìm thấy cửa hàng','error');
      $form_state['redirect'] = 'store';
    }
}

function adstrue_vendor_store_validate($form, &$form_state) {
  $name_store = $form_state['values']['name_store'];
  $store_masap = $form_state['values']['store_masap'];
  $store_id = $form_state['values']['hidden_tid'];
  $company_id = $form_state['values']['company_id'];
  if($company_id == 0) {
      form_set_error('company_id',t('Vui lòng chọn công ty'));
  }
  if($store_masap) {
      $query = db_select('field_data_field_store_sap','sap');
      $query->condition('field_store_sap_value',$store_masap);
      $query->fields('sap');
      $result = $query->execute();
      if($result->rowCount()) {
          foreach($result as $row) {
            if($store_id !== $row->entity_id) {
                form_set_error('store_masap','Mã SAP đã tồn tại');
            }
          }
      }
  }
  if($name_store == '') {
    form_set_error('',t('Bạn vui lòng nhập tên cửa hàng'));
  }
}
/*
* Submit the form region
*/
function adstrue_vendor_store_submit($form,&$form_state) {
  global $user;
  $tid = $form_state['values']['hidden_tid'];
  $name_store = $form_state['values']['name_store'];
  $store_description = $form_state['values']['store_description'];
  $province_id = $form_state['values']['province_id'];
  $masap = $form_state['values']['store_masap'];
  $store_email = $form_state['values']['store_email'];
  $contact_number = $form_state['values']['contact_number'];
  $contact_mobile = $form_state['values']['contact_mobile'];
  $name_person = $form_state['values']['name_person'];
  $store_note = $form_state['values']['store_note'];

  if($province_id == 0) {
     $province_id = null;
  }
  $region_id = $form_state['values']['region_id'];
  if($region_id == 0) { 
    $region_id = null;
  }
  $company_id = $form_state['values']['company_id'];
  if($company_id == 0) {
     $company_id = null;
  } 
  $cur_time = time();

  if(isset($tid) && $tid !== '') {
    $term = taxonomy_term_load($tid);
    $term->name = $name_store;

    $term->description = $store_description;
    $term->field_company_store_id['und'][0]['tid'] = $company_id;
    $term->field_province_store_id['und'][0]['tid'] = $province_id;
    $term->field_region_store_id['und'][0]['tid'] = $region_id;
    $term->field_store_sap['und'][0]['value'] = $masap;
    $term->field_store_contact_person['und'][0]['value'] = $name_person;
    $term->field_store_contact_number['und'][0]['value'] = $contact_number;
    $term->field_store_contact_mobile['und'][0]['value'] = $contact_mobile;
    $term->field_store_email['und'][0]['value'] = $store_email;
    $term->field_store_user['und'][0]['uid'] = $user->uid;
    $term->field_store_updated['und'][0]['value'] = $cur_time;
    $term->field_store_noting['und'][0]['value'] = $store_note;

    $status =  taxonomy_term_save($term);
    if($status == 2)  {
        drupal_set_message(t('Cập nhật cửa hàng thành công'));
        $form_state['redirect'] = 'store';
        return;
    } else {
        drupal_set_message(t('Không thể cập nhật cửa hàng','error'));
        $form_state['redirect'] = 'store';
        return;
    }
  }
  $voca = taxonomy_vocabulary_machine_name_load('store');
  $vid = $voca->vid;
  //Create term object to add to the dabase
  $term = new stdClass;
  $term->name = $name_store;
  $term->description = $store_description;

  $term->field_province_store_id['und'][0]['tid'] = $province_id;
  $term->field_region_store_id['und'][0]['tid'] = $region_id;
  $term->field_company_store_id['und'][0]['tid'] = $company_id;
  $term->field_store_sap['und'][0]['value'] = $masap;

  $term->field_store_contact_person['und'][0]['value'] = $name_person;
  $term->field_store_contact_number['und'][0]['value'] = $contact_number;
  $term->field_store_contact_mobile['und'][0]['value'] = $contact_mobile;
  $term->field_store_email['und'][0]['value'] = $store_email;
  $term->field_store_user['und'][0]['uid'] = $user->uid;
  $term->field_store_updated['und'][0]['value'] = $cur_time;
  $term->field_store_noting['und'][0]['value'] = $store_note;
  $term->vid = $vid;

  taxonomy_term_save($term);

  if($term->tid) {
    drupal_set_message('Đã tạo một cửa hàng');
    $form_state['redirect'] = 'store';
  } else {
      drupal_set_message(t('Không thể tạo cửa hàng','error'));
      $form_state['redirect'] = 'store';
  }
}


function store_adstrue_vendor($form, &$form_state) {
    global $user,$base_url; 
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Mã SAP'),
        array('data' => 'Tên cửa hàng'),
        array('data' => 'Địa chỉ'),
        array('data' => 'Người liên hệ'),
        array('data' => 'Số điện thoại'),
        array('data' => 'Email'),
        array('data' => 'Tên tỉnh thành'),
        array('data' => 'Vùng miền'),
        array('data' => 'Cập nhật'),
        array('data' => 'Người cập nhật'),
        array('data' => 'Ghi chú'),
        array('data' => 'Thao tác')
    );

     // List all company to filter
    $voca = taxonomy_vocabulary_machine_name_load('company');
    $vid = $voca->vid;
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $vid));
    $cSelect = array(0=>t('Chọn công ty'));
    foreach($companies as $company) {
        $cSelect[$company->tid] = $company->name;
    }

    $return = show_store_list_table();
    $rows = $return['rows'];
    $total = $return['total'];
    $message = '<h4>Tổng số cửa hàng : <b>'.$total.'</b></h4>';

    if(isset($_GET['company']) && (int)$_GET['company'] > 0) {
        $company_id = $_GET['company'];
        $rgSe = filter_region_by_company($company_id);
    }
    if(isset($_GET['region']) && (int)$_GET['region'] > 0) {
        $region_id = $_GET['region'];

        $prSe  = filter_province_by_region($region_id);
    }
    if(isset($_GET['province']) && (int)$_GET['province'] > 0) {
        $province_id = $_GET['province'];
        $prSe  = filter_province_by_region($region_id);
    }

    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
        $keyword = $_GET['keyword'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
    }
    if(isset($_GET['ma_sap']) && !empty($_GET['ma_sap'])) {
        $ma_sap = $_GET['ma_sap'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
    }
    
    // Xay dung form
    $form = array();
    $form['danhsach_store'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="">',
        '#suffix' => '</div>'
    );

    $form['danhsach_store']['header_filter'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('box','box-header')
        ),
    );
    $form['danhsach_store']['header_filter']['company'] = array(
         '#type' => 'select',
         '#title' => t('Công ty'),
         '#options' => $cSelect,
         '#ajax' => array(
            'callback' => 'store_load_region_by_company_table',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
          ),
         '#default_value' => isset($company_id) ? $company_id : 0,
         '#prefix' => '<div class="col-md-2 box-search-item">',
         '#suffix' => '</div>',
    );

    $form['danhsach_store']['header_filter']['region'] = array(
         '#type' => 'select',
         '#title' => t('Vùng miền'),
         '#options' => (isset($rgSe)) ? $rgSe : array(0 => t('Chọn vùng miền')),
         '#ajax' => array(
            'callback' => 'store_load_province_by_region_table',
            'method' =>  'replace',
            'wrapper' => 'province-wrapper',
          ),
          '#default_value' => isset($region_id) ? $region_id : 0,
          '#prefix' => '<div class="col-md-2 box-search-item"><div id="region-wrapper">',
          '#suffix' => '</div></div>',
          '#validated' => TRUE,
    );

    $form['danhsach_store']['header_filter']['province'] = array(
         '#type' => 'select',
         '#title' => t('Tỉnh thành'),
         '#options' => isset($prSe) ? $prSe : array(0 => t('Chọn tỉnh thành')),
         '#default_value' => isset($province_id) ? $province_id : 0,
         '#prefix' => '<div class="col-md-2 box-search-item"><div id="province-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE
    );

    $form['danhsach_store']['header_filter']['keyword'] = array(
        '#type' => 'textfield',
        '#size' => 60,
        '#title' =>t('Tên cửa hàng'),
        '#attributes' => array(
          'placeholder' => 'Tìm cửa hàng',
        ),
        '#default_value' => isset($keyword) ? $keyword : null, 
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-4 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['danhsach_store']['header_filter']['ma_sap'] = array(
        '#type' => 'textfield',
        '#size' => 60,
        '#title' => t('Mã SAP'),
        '#attributes' => array(
          'placeholder' => 'Tìm mã SAP',
        ),
        '#default_value' => isset($ma_sap) ? $ma_sap : null, 
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['danhsach_store']['header_filter']['create_store'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/store/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Tạo cửa hàng </a>',
        '#prefix' => '<div class="col-md-2 box-search-item pull-right pull-right">',
        '#suffix' => '</div>',
    );
    $form['danhsach_store']['header_filter']['export_excel'] = array(
        '#type' => 'submit',
        '#weight' => 3,
        '#value' => 'Xuất excel',
        '#prefix' => '<div class="col-md-2 box-search-item pull-right">',
        '#suffix' => '</div>',
        '#submit' => array('store_export_excel_submit')
    );
    

    $form['danhsach_store']['header_filter']['submit_search'] = array(
        '#type' => 'submit',
        '#value' => t('Tìm kiếm'),
        '#prefix' => '<div class="col-md-2 box-search-item pull-right">',
        '#suffix' => '</div>',
        '#submit' => array('filter_store_by_search'),
    );
    $form['danhsach_store']['search_summary'] = array(
        '#type' => 'item',
        '#markup' => '<div class="callout callout-success">'.$message.'</div>',
        '#prefix' => '<div class="col-md-12" style="margin-top : 30px;">',
        '#suffix' => '</div>'
    ); 

    $form['danhsach_store']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['danhsach_store']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4,
    );

    return $form;
}

// Xuat excel
function store_export_excel_submit($form,&$form_state) {
    $path_class = drupal_get_path('module','adstrue_vendor') .'/export_data_excel.php';
    include $path_class;
    $values = $form_state['values'];
    $result = show_store_list_table($values,0);

    $exporter = new ExportDataExcel('browser', 'store_list.xls');
    $exporter->initialize();
    $exporter->addRow(array('STT','Mã SAP','Tên Cửa hàng','Địa chỉ','Người liên hệ','Điện thoại cố định','Điện thoại cửa hàng trưởng','Email','Tên tỉnh thành','Vùng miền','Cập nhật','Người cập nhật'));

    if($result) {
        if($result->rowCount()) {
              $i = 1;
              foreach($result as $row) {
                  $term = taxonomy_term_load($row->tid);
                  $ma_sap = (isset($term->field_store_sap['und'][0]['value'])) ? $term->field_store_sap['und'][0]['value'] : '';
                  $name_person = (isset($term->field_store_contact_person['und'][0]['value'])) ? $term->field_store_contact_person['und'][0]['value'] : '';
                  $contact_number = (isset($term->field_store_contact_number['und'][0]['value'])) ? $term->field_store_contact_number['und'][0]['value'] : '';
                  $contact_mobile = (isset($term->field_store_contact_mobile['und'][0]['value'])) ? $term->field_store_contact_mobile['und'][0]['value'] : '';
                  
                  $store_email = (isset($term->field_store_email['und'][0]['value'])) ? $term->field_store_email['und'][0]['value'] : '';

                  $store_note = (isset($term->field_store_noting['und'][0]['value'])) ? $term->field_store_noting['und'][0]['value'] : '';

                  $store_updated = (isset($term->field_store_updated['und'][0]['value'])) ? $term->field_store_updated['und'][0]['value'] : '';
                  if($store_updated) {
                     $store_updated = date('H:i:s d/m/Y',$store_updated); 
                  }
                  $province_name = '';
                  $province_id = (isset($term->field_province_store_id['und'][0]['tid'])) ? $term->field_province_store_id['und'][0]['tid'] : '';
                  if($province_id) {
                      $province = taxonomy_term_load($province_id);
                      $province_name = $province->name;
                  }
                  $region_name = '';
                  $region_id = (isset($term->field_region_store_id['und'][0]['tid'])) ? $term->field_region_store_id['und'][0]['tid'] : '';
                  if($region_id) {
                      $region = taxonomy_term_load($region_id);
                      $region_name = $region->name;
                  }

                  $user_name_update = '';
                  $user_update = (isset($term->field_store_user['und'][0]['uid'])) ? $term->field_store_user['und'][0]['uid'] : '';
                  if($user_update) {
                      $user_obj = user_load($user_update);
                      $user_name_update = $user_obj->name; 
                  }

                  $row = array(
                      $i,
                      $ma_sap,
                      $term->name,
                      $term->description,
                      $name_person,
                      $contact_number,
                      $contact_mobile,
                      $store_email,
                      $province_name,
                      $region_name,
                      $store_updated,
                      $user_name_update,
                  );
                  $exporter->addRow($row);
                  $i++;
              }
        }
    }
    $exporter->finalize();
    exit();
}

function filter_store_by_search($form,&$form_state) {
    global $base_url;
    $values  = $form_state['values'];
    $get = '?';
    foreach($values as $key => $val) {
        if($key !== 'submit_search'  && $key !== 'form_build_id' && $key !== 'form_token' && $key !== 'form_id' && $key !== 'op') {
            $get .= '&'.$key.'='.$val;
        }
    }
    $form_state['redirect'] = $base_url.'/store/'.$get;
}

function show_store_list_table($values = '',$is_pager = 1) {
    global $base_url;
    $limit = 50;
    $voca = taxonomy_vocabulary_machine_name_load('store');
    $vid = $voca->vid;
    if($is_pager) {
        $query = db_select('taxonomy_term_data','tdata')->extend('PagerDefault')->distinct()->limit($limit);
    } else {
        $_GET = $values;
        $query = db_select('taxonomy_term_data','tdata');
    }
    $query->leftJoin('field_data_field_store_sap','stsap','stsap.entity_id = tdata.tid');
    $query->condition('tdata.vid',$vid);

    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
        $keyword = $_GET['keyword'];
        $query->condition('tdata.name','%'.db_like($keyword).'%','LIKE');
        $_GET['company'] = 0;
        $_GET['region'] = 0;
        $_GET['province'] = 0;
        $_GET['ma_sap'] = '';
    }

    if(isset($_GET['ma_sap']) && !empty($_GET['ma_sap'])) {
        $ma_sap = $_GET['ma_sap'];
        $query->condition('stsap.field_store_sap_value','%'.db_like($ma_sap).'%','LIKE');
        $_GET['company'] = 0;
        $_GET['region'] = 0;
        $_GET['province'] = 0;
    }
    if(isset($_GET['company']) && (int)$_GET['company'] > 0) {
        $company_id = $_GET['company'];
        $query->leftJoin('field_data_field_company_store_id','stcp','stcp.entity_id = tdata.tid');
        $query->condition('stcp.field_company_store_id_tid',$company_id);
    }
    if(isset($_GET['region']) && (int)$_GET['region'] > 0) {
        $region_id = $_GET['region'];
        $query->leftJoin('field_data_field_region_store_id','strg','strg.entity_id = tdata.tid');
        $query->condition('strg.field_region_store_id_tid',$region_id);
    }
    if(isset($_GET['province']) && (int)$_GET['province'] > 0) {
        $province_id = $_GET['province'];
        $query->leftJoin('field_data_field_province_store_id','stpr','stpr.entity_id = tdata.tid');
        $query->condition('stpr.field_province_store_id_tid',$province_id);
    }
    if(isset($_GET['page']) && (int)$_GET['page'] > 0) {
        $page  = (int)$_GET['page'];
        $i = $limit*$page + 1;
    } else {
        $i = 1;
    }

    $query->fields('tdata',array('tid'));
    $query->orderBy('stsap.field_store_sap_value','DESC');
    $total = $query->countQuery()->execute()->fetchField();
    $result = $query->execute();
    // Truong hop xuat khong co phan trang nhu la: xuat excel
    if($is_pager == 0) {
        return $result;
    }
    if($result->rowCount()) {
        foreach($result as $row) {
            $term = taxonomy_term_load($row->tid);
            $province_id = (isset($term->field_province_store_id['und'][0]['tid'])) ? $term->field_province_store_id['und'][0]['tid'] : 0;
            $province = taxonomy_term_load($province_id);

            $region_id = (isset($term->field_region_store_id['und'][0]['tid'])) ? $term->field_region_store_id['und'][0]['tid'] : 0;
            $region = taxonomy_term_load($region_id);

            $ma_sap = (isset($term->field_store_sap['und'][0]['value'])) ? $term->field_store_sap['und'][0]['value'] : '';

            $address_store = (isset($term->description)) ? $term->description : '';
            $name_person = (isset($term->field_store_contact_person['und'][0]['value'])) ? $term->field_store_contact_person['und'][0]['value'] : '';
            $contact_number = (isset($term->field_store_contact_number['und'][0]['value'])) ? $term->field_store_contact_number['und'][0]['value'] : '';
            if($contact_number) {
                $contact_number = '<p>
                      <strong>Máy bàn: </strong>'.$contact_number.'
                      </p>';
            }
            $contact_mobile = (isset($term->field_store_contact_mobile['und'][0]['value'])) ? $term->field_store_contact_mobile['und'][0]['value'] : '';
            if($contact_mobile) {
                $contact_mobile = '<p>
                      <strong>Di động: </strong>'.$contact_mobile.'
                      </p>';
            }
            $store_email = (isset($term->field_store_email['und'][0]['value'])) ? $term->field_store_email['und'][0]['value'] : '';
            $phone = $contact_number.$contact_mobile;

            $user_name = '';
            $user_id = (isset($term->field_store_user['und'][0]['uid'])) ? $term->field_store_user['und'][0]['uid'] : '';
            if($user_id) {
                $user = user_load($user_id);
                $user_name = $user->name;
            }
            $store_note = (isset($term->field_store_noting['und'][0]['value'])) ? $term->field_store_noting['und'][0]['value'] : '';

            $changed = (isset($term->field_store_updated['und'][0]['value'])) ? $term->field_store_updated['und'][0]['value'] : '';
            if($changed) {
                $changed = date('H:i:s d/m/Y',$changed);
            }


            $rows[] = array(
                $i,
                $ma_sap,
                $term->name,
                '<p style="max-width: 250px">'.$address_store.'</p>',
                $name_person,
                $phone,
                $store_email,
                (isset($province) && $province) ? $province->name : '',
                (isset($region) && $region) ? $region->name : '',
                $changed,
                $user_name,
                $store_note,
                 '<div class="btn-group">
                      <button type="button" class="btn btn-success">Thao tác</button>
                      <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                          <span class="caret"></span>
                          <span class="sr-only">Toggle Dropdown</span>
                      </button>
                      <ul class="dropdown-menu" role="menu">
                          <li><a href="'.$base_url.'/store/edit/'.$term->tid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                          <li><a href="'.$base_url.'/store/delete/'.$term->tid.'" ><i class="fa fa-trash"></i> Xóa</a></li>
                      </ul>
                  </div>'
            );
            $i++;
        }
    }
    return ['rows' => $rows, 'total' => $total];

}

function adstrue_vendor_import_store_validate($form,&$form_state) {
    $company_id = $form_state['values']['company_id'];
    if(!$company_id) {
        form_set_error('company_id','Vui lòng chọn công ty');
    }
}
function adstrue_vendor_import_store_submit($form,&$form_state) {
    global $user;
    // Lay link file excel
    // Load file excel vao 1 mang
    $file_info = $form_state['values']['file_store_to_upload'];
    $company_id = $form_state['values']['company_id'];
    $region_id = $form_state['values']['region_id'];
    $province_id = $form_state['values']['province_id'];
    // Lay dia chi file
    $path_module = drupal_get_path('module','adstrue_vendor');
    if(isset($file_info) && $file_info) {
        $excel_file =  file_load($file_info);
        if(!empty($excel_file->fid)) {
            // Lay full duong dan file excel
            $file_excel_url = drupal_realpath($excel_file->uri);
            $path_excel_lib = 'Classes/PHPExcel.php';
            set_include_path($path_module);

            require_once "Classes/PHPExcel.php";
            $excelReader = PHPExcel_IOFactory::createReaderForFile($file_excel_url);
            $excelObj = $excelReader->load($file_excel_url);
            $worksheet = $excelObj->getSheet(0);
            $lastRow = $worksheet->getHighestRow();
            $lastCol = $worksheet->getHighestColumn();
            $data = $worksheet->toArray(null,true,true,true);
            $sap_arr = array_column($data, 'B');
            $exist_sap = [];
            if(count($sap_arr)) {
                $check_db = db_select('field_data_field_store_sap','sap')
                        ->fields('sap')
                        ->condition('sap.field_store_sap_value',$sap_arr,'IN')
                        ->execute();

            
                if($check_db->rowCount()) {
                    foreach($check_db as $row) {
                        $exist_sap[] = $row->field_store_sap_value;
                    }
                }
            }

            // foreach qua mang data de gn gia tri cho store
            array_shift($data);
            $voca = taxonomy_vocabulary_machine_name_load('store');
            $vid = $voca->vid;
            $cur_time = time();
            foreach($data as $element) {
                // Co ma SAP trung thi qua row khac
                if(in_array($element['B'], $exist_sap)) {
                    continue;
                }
                $term = new stdClass();
                $term->name = $element['C'];
                $term->description = $element['D'];

                if($province_id) {
                  $term->field_province_store_id['und'][0]['tid'] = $province_id;
                }
                if($region_id) {
                    $term->field_region_store_id['und'][0]['tid'] = $region_id;
                }
                if($company_id) {
                    $term->field_company_store_id['und'][0]['tid'] = $company_id;
                }
                
                $term->field_store_sap['und'][0]['value'] = $element['B'];

                $term->field_store_contact_person['und'][0]['value'] = $element['E'];
                $term->field_store_contact_number['und'][0]['value'] = $element['F'];
                $term->field_store_contact_mobile['und'][0]['value'] = $element['G'];
                $term->field_store_email['und'][0]['value'] = $element['H'];
                $term->field_store_user['und'][0]['uid'] = $user->uid;
                $term->field_store_updated['und'][0]['value'] = $cur_time;
                $term->field_store_noting['und'][0]['value'] = $element['I'];
                $term->vid = $vid;

                taxonomy_term_save($term);
            }
            if(count($exist_sap) > 0) {
                $exist_sap = implode(', ', $exist_sap);
                drupal_set_message('Các mã SAP đã tồn tại: '.$exist_sap.' .Import thành công cửa hàng');
            } else {
                drupal_set_message('Import cửa hàng thành công.');
            }

        }
    }
}