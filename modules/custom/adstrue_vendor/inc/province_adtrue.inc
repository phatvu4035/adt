<?php 
/* 
* Build form to add provice 
*/
function adstrue_vendor_add_provice() {
    return build_form_province();
}

function adstrue_vendor_update_provice($form,&$form_state,$value) {
	return build_form_province($value);
}

function build_form_province($tid = 0) {
    drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');


    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;
   // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }
    
    // No phai thuoc vocabulary la tinh thanh moi duoc hien thi tren form
    $term = taxonomy_term_load($tid);
    $voca = taxonomy_vocabulary_machine_name_load('province');
    $vid = $voca->vid;
    if($term) {
        if($term->vid !== $vid) {
          drupal_not_found();
        }
    }
    
    if(isset($term->field_company_province_id) && !empty($term->field_company_province_id)) {
      $company_id = $term->field_company_province_id['und'][0]['tid'];
      $rgSe = filter_region_by_company($company_id);
      
    } 
    if(isset($term->field_region_provice_id) && !empty($term->field_region_provice_id)) {
      $region_id = $term->field_region_provice_id['und'][0]['tid'];
    }

    $form['fieldset_all'] = array(
        '#type' => 'container',
        '#weight' => 3, 
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr">',
        '#suffix' => '</div>'
     );
    $form['fieldset_all']['fieldset_info'] = array(
      '#type' => 'container',
      '#weight' => 1,
        '#attributes' => array(
            'class' => array('box')
        ),
        '#prefix' => '<div class="col-md-6 wrPl">',
        '#suffix' => '</div>'
   );
   $form['fieldset_all']['fieldset_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
   );
   $form['fieldset_all']['fieldset_info']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Nhập thông tin tỉnh thành</h3>'
    );
    $form['fieldset_all']['hidden_tid'] = array(
      '#type' => 'hidden',
      '#title' => t('tid hidden'),
      '#default_value' => (isset($term->tid)) ? $term->tid : '',

    );
    $form['fieldset_all']['fieldset_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
  $form['fieldset_all']['fieldset_info']['body']['name_province'] = array(
      '#type' => 'textfield',
      '#title' => t('Tên tỉnh thành'),
      '#default_value' => (isset($term->name)) ? $term->name : '',
      '#required' => TRUE,
    );
    $form['fieldset_all']['fieldset_info']['body']['province_description'] = array(
      '#type' => 'textarea',
      '#title' => t('Mô tả'),
      '#default_value' => (isset($term->description)) ? $term->description : '',
    );
    $form['fieldset_all']['fieldset_property'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box')
        ),
        '#prefix' => '<div class="col-md-6 wrPl">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['fieldset_property']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
   );
    $form['fieldset_all']['fieldset_property']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Nhập nơi quản lí</h3>'
    );
    $form['fieldset_all']['fieldset_property']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['fieldset_property']['body']['company_id'] = array(
      '#type' => 'select',
      '#title' => t('Tên công ty'),
      '#options' => $cSelect,
      '#ajax' => array(
        'callback' => 'load_region_by_company',
        'method' =>'replace',
        'wrapper' => 'region-wrapper',
      ),
      '#default_value' => (isset($company_id)) ? $company_id : 0,
      '#required' => TRUE,
    );
    $form['fieldset_all']['fieldset_property']['body']['region_id'] = array(
      '#type' => 'select',
      '#prefix' => '<div id="region-wrapper">',
      '#title' => t('Chọn vùng miền'),
      '#options' => (isset($rgSe) && $rgSe) ? $rgSe : array(0 => t('Chọn')),
      '#default_value' => (isset($region_id)) ? $region_id : 0,
      '#suffix' => '</div>',
      '#validated' => TRUE
    );

    $form['fieldset_all']['footer'] = array(
        '#type' => 'container',
        '#weight' => 6,
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );
    $form['fieldset_all']['footer']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Lưu lại'),
        '#prefix' => '<div class="pull-right">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('btn','btn-primary')
        ),
    );

    $form['#validate'][] = 'adstrue_vendor_provice_validate';
    $form['#submit'][] = 'adstrue_vendor_provice_submit';
    return  $form;
}

/*
* Delete province taxonomy form
*/
function delete_adstrue_vendor_province($form,&$form_state,$value) {
    $form = array();
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => '<h4>Vui lòng xác nhận để xóa tỉnh thành</h4>',
    );
    $form['province_id'] = array(
        '#type' => 'hidden',
        '#value' => $value,
    );
    return confirm_form($form,'Vui lòng xác nhận để xóa tỉnh thành','province',t(''),t('Xác nhận'),t('Hủy bỏ'));
}

function delete_adstrue_vendor_province_submit($form,&$form_state) {
      $tid = $form_state['values']['province_id'];
      $tax = taxonomy_term_load($tid);

      $voca = taxonomy_vocabulary_machine_name_load('province');
      $vid = $voca->vid;
      if($tax->vid == $vid && $tax) {
        $stSe = filter_store_by_province($tid);
        if(count($stSe) > 1) {
          drupal_set_message('Tỉnh thành chứa các cửa hàng không thể xóa','error');
          $form_state['redirect'] = 'province';
          return;
        }
        taxonomy_term_delete($tid);
        drupal_set_message('Đã xóa tỉnh thành');
        $form_state['redirect'] = 'province';
      } else {
        drupal_set_message('Không tìm thấy tỉnh thành','error');
        $form_state['redirect'] = 'province';
      }
}

/* 
* Validate the form province when user submit
*/
function adstrue_vendor_provice_validate($form, &$form_state) {
    drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');
    
    $name_province = $form_state['values']['name_province'];
    $company_id = $form_state['values']['company_id'];
    if($company_id == 0) {
        form_set_error('company_id',t('Vui lòng chọn công ty'));
    }

    $province_description = $form_state['values']['province_description'];
    $region_id = $form_state['values']['region_id'];
    $company_id = $form_state['values']['company_id'];
    if($name_province == '') {
      form_set_error('Vui lòng nhập tên tỉnh thành');
    }
}

/*
* Submit the form region
*/
function adstrue_vendor_provice_submit($form,&$form_state) {
  $tid = $form_state['values']['hidden_tid'];
  $name_province = $form_state['values']['name_province'];
  $province_description = $form_state['values']['province_description'];
  $region_id = $form_state['values']['region_id'];
  if($region_id == 0) { 
    $region_id = null;
  }
  $company_id = $form_state['values']['company_id'];
  if($company_id == 0) {
     $company_id = null;
  }

  if(isset($tid) && $tid) {
    $term = taxonomy_term_load($tid);
    $term->name = $name_province;
    $term->description = $province_description;
    $term->field_region_provice_id['und'][0]['tid'] = $region_id;
    $term->field_company_province_id['und'][0]['tid'] = $company_id;
    $status = taxonomy_term_save($term);
    if($status == 2) {
      drupal_set_message(t('Cập nhật tỉnh thành thành công'));
      $form_state['redirect'] = 'province';
    } else {
      drupal_set_message(t('Không thể cập nhật tỉnh thành','error'));
      $form_state['redirect'] = 'province';
    }
    return;
  }
  $voca = taxonomy_vocabulary_machine_name_load('province');
  $vid = $voca->vid;
  //Create term object to add to the dabase
  $term  = new stdClass;
  $term->name = $name_province;
  $term->description = $province_description;
  $term->field_region_provice_id['und'][0]['tid'] = $region_id;
  $term->field_company_province_id['und'][0]['tid'] = $company_id;
  $term->vid = $vid;
  $status = taxonomy_term_save($term);
  if($term->tid) {
    drupal_set_message('Đã tạo một tỉnh thành');
    $form_state['redirect'] = 'province';
  } else {
    drupal_set_message('Không thể tạo tỉnh thành','error');
    $form_state['redirect'] = 'province';
  }
}


function province_adstrue_vendor($form, &$form_state) {
    global $user,$base_url;
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Tên tỉnh thành'),
        array('data' => 'Vùng miền'),
        array('data' => 'Công ty'),
        array('data' => 'Thao tác')
    );
    //Lay danh sach cac tinh
    $rows = show_province_list_table();
    // List all company to filter
    $voca = taxonomy_vocabulary_machine_name_load('company');
    $vid = $voca->vid;
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $vid));
    $cSelect = array(0=>t('Chọn công ty'));
    foreach($companies as $company) {
        $cSelect[$company->tid] = $company->name;
    }

    if(isset($_GET['company']) && (int)$_GET['company'] > 0) {
        $company_id = $_GET['company'];
        $rgSe = filter_region_by_company($company_id);
    }
    if(isset($_GET['region']) && (int)$_GET['region'] > 0) {
        $company_id = $_GET['company'];
        $region_id = $_GET['region'];
        $rgSe = filter_region_by_company($company_id);
    }
    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
        $keyword = $_GET['keyword'];
        $company_id = 0;
        $region_id = 0;
    }

    // Xay dung form
    $form = array();
    $form['danhsach_province'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

     $form['danhsach_province']['header_filter'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('box','box-header')
        ),
    );
    $form['danhsach_province']['header_filter']['company'] = array(
         '#type' => 'select',
         '#title' => t('Công ty'),
         '#options' => $cSelect,
         '#ajax' => array(
            'callback' => 'province_load_region_by_company_table',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
          ),
         '#default_value' => isset($company_id) ? $company_id : 0,
         '#prefix' => '<div class="col-md-2 box-search-item">',
         '#suffix' => '</div>',
    );

    $form['danhsach_province']['header_filter']['region'] = array(
         '#type' => 'select',
         '#title' => t('Vùng miền'),
         '#options' => (isset($rgSe)) ? $rgSe : array(0 => t('Chọn vùng miền')),
         '#ajax' => array(
            'callback' => 'province_load_province_by_region_table',
            'method' =>  'replace',
            'wrapper' => 'province-wrapper',
          ),
          '#default_value' => isset($region_id) ? $region_id : 0,
          '#prefix' => '<div class="col-md-2 box-search-item"><div id="region-wrapper">',
          '#suffix' => '</div></div>',
          '#validated' => TRUE,
    );
     $form['danhsach_province']['header_filter']['keyword'] = array(
        '#type' => 'textfield',
        '#size' => 60,
        '#title' => t('Tên tỉnh thành'),
        '#attributes' => array(
          'placeholder' => 'Tìm kiếm',
        ),
        '#default_value' => isset($keyword) ? $keyword : null,
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
    );

    $form['danhsach_province']['header_filter']['submit_search'] = array(
        '#type' => 'submit',
        '#value' => t('Tìm kiếm'),
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
        '#submit' => array('filter_province_by_search'),
    );

    $form['danhsach_province']['header_filter']['create_province'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/province/add" class="btn btn-success "><i class="fa fa-plus"></i> Tạo tỉnh thành </a>',
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );

    $form['danhsach_province']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['danhsach_province']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4,
    );
    return $form;
}


function show_province_list_table() {
    global $base_url;
    $limit = 20;
    $voca = taxonomy_vocabulary_machine_name_load('province');
    $vid = $voca->vid;
    $query = db_select('taxonomy_term_data','tdata')->extend('PagerDefault')->distinct()->limit($limit);
    $query->condition('tdata.vid',$vid);

    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
        $keyword = $_GET['keyword'];
        $query->condition('tdata.name','%'.db_like($keyword).'%','LIKE');
        $_GET['company'] = 0;
        $_GET['region'] = 0;
    }
    if(isset($_GET['company']) && $_GET['company'] > 0){
        $company_id = $_GET['company'];
        $query->leftJoin('field_data_field_company_province_id','cprid','cprid.entity_id = tdata.tid');
        $query->condition('cprid.field_company_province_id_tid',$company_id);
    }
    if(isset($_GET['region']) && $_GET['region'] > 0){
        $region_id = $_GET['region'];
        $query->leftJoin('field_data_field_region_provice_id','rprid','rprid.entity_id = tdata.tid');
        $query->condition('rprid.field_region_provice_id_tid',$region_id);
    }
    if(isset($_GET['page']) && (int)$_GET['page'] > 0) {
        $page  = (int)$_GET['page'];
        $index = $limit*$page + 1;
    } else {
        $index  = 1;
    }

    $query->fields('tdata',array('tid'));
    $result = $query->execute();
    $rows = array();
    if($result->rowCount()) {
        foreach($result as $row) {
            $term = taxonomy_term_load($row->tid);
            $region_id = (isset($term->field_region_provice_id['und'][0]['tid'])) ? $term->field_region_provice_id['und'][0]['tid'] : 0;
            $region = taxonomy_term_load($region_id);
           
            $company_id = (isset($term->field_company_province_id['und'][0]['tid'])) ? $term->field_company_province_id['und'][0]['tid'] : 0;
            $company = taxonomy_term_load($company_id);

            $rows[] = array(
                $index,
                $term->name,
                (isset($region) && $region) ? $region->name : '',
                (isset($company) && $company) ? $company->name : '',
                 '<div class="btn-group">
                      <button type="button" class="btn btn-success">Thao tác</button>
                      <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                          <span class="caret"></span>
                          <span class="sr-only">Toggle Dropdown</span>
                      </button>
                      <ul class="dropdown-menu" role="menu">
                          <li><a href="'.$base_url.'/province/edit/'.$term->tid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                          <li><a href="'.$base_url.'/province/delete/'.$term->tid.'" ><i class="fa fa-trash"></i> Xóa</a></li>
                      </ul>
                  </div>'
            );
            $index++;
        }
    }
    return $rows;

}

function filter_province_by_search($form,&$form_state) {
    global $base_url;
    $values  = $form_state['values'];
    $get = '?get=1';
    foreach($values as $key => $val) {
        if($key !== 'submit_search'  && $key !== 'form_build_id' && $key !== 'form_token' && $key !== 'form_id' && $key !== 'op') {
            $get .= '&'.$key.'='.$val;
        }
    }
    $form_state['redirect'] = $base_url.'/province/'.$get;
}