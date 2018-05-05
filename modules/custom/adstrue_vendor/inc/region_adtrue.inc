<?php 
/* 
* Build form to add region 
*/
function adstrue_vendor_add_region($form,&$form_state) {
	return build_region_form(); 
}

function adstrue_vendor_update_region($form,&$form_state,$value) {
	return build_region_form($value);
}

function build_region_form($tid = 0) {
	 drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');

    $cVoca = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoca->vid;

     // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $companies = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }

    $term = taxonomy_term_load($tid);
    //Check xem nó có thuocj vid là vùng miền không
    $voca = taxonomy_vocabulary_machine_name_load('region');
    $vid = $voca->vid;
    if($term) {
      if($term->vid !== $vid) {
        drupal_not_found();
      }
    }
    if(isset($term->field_region_company_id) && !empty($term->field_region_company_id)) {
      $company_id = $term->field_region_company_id['und'][0]['tid'];
    }

	// Xây dựng form
     $form['hidden_tid'] = array(
        '#type' => 'hidden',
        '#title' => t('Tên vùng miền'),
        '#default_value' => (isset($term) && $term) ? $term->tid : '',
     );

     $form['fieldset_info'] = array(
        '#type' => 'container',
        '#weight' => 3, 
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr">',
        '#suffix' => '</div>'
     );
     $form['fieldset_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
      );
      $form['fieldset_info']['header']['header_title'] = array(
          '#type' => 'item',
          '#markup' => '<h3>Nhập thông tin vùng miền</h3>'
      );
      $form['fieldset_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
      );
     $form['fieldset_info']['body']['name_region'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên vùng miền'),
        '#default_value' => (isset($term->name) && $term) ? $term->name : '',
        '#prefix' => '<div class="center_small_form">',
        '#suffix' => '</div>',
        '#required' => TRUE,
     );
     $form['fieldset_info']['body']['region_description'] = array(
        '#type' => 'textarea',
        '#title' => t('Mô tả'),
        '#default_value' => (isset($term->description) && $term) ? $term->description : '',
        '#prefix' => '<div class="center_small_form">',
        '#suffix' => '</div>'
     );
     $form['fieldset_info']['body']['company_id'] = array(
        '#type' => 'select',
        '#title' => t('Tên công ty'),
        '#options' => $cSelect,
        '#default_value' =>(isset($company_id)) ? $company_id : 0,
        '#description' => t('Chọn công ty quản lí vùng miền'),
        '#prefix' => '<div class="center_small_form">',
        '#suffix' => '</div>',
        '#required' => TRUE,
     );
     $form['fieldset_info']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );

     $form['fieldset_info']['footer']['submit'] = array(
      '#type' => 'submit',
      '#weight' => 6,
      '#value' => t('Lưu lại'),
      '#prefix' => '<div class="center_small_form"><div class="pull-right">',
        '#suffix' => '</div></div>',
        '#attributes' => array(
            'class' => array('btn','btn-primary')
      ),
    );
    $form['#validate'][] = 'adstrue_vendor_region_validate';
    $form['#submit'][] = 'adstrue_vendor_region_submit';
     return  $form;
}

/*
* Delete region taxonomy form
*/
function delete_adstrue_vendor_region($form,&$form_state,$value) {
    $form = array();
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => '<h4>Vui lòng xác nhận để xóa vùng miền</h4>',
    );
    $form['region_id'] = array(
        '#type' => 'hidden',
        '#value' => $value,
    );
    return confirm_form($form,'Vui lòng xác nhận để xóa vùng miền','region',t(''),t('Xác nhận'),t('Hủy bỏ'));
}

function delete_adstrue_vendor_region_submit($form,&$form_state) {
    $tid = $form_state['values']['region_id'];
    $tax = taxonomy_term_load($tid);

    $voca = taxonomy_vocabulary_machine_name_load('region');
    $vid = $voca->vid;
    if($tax->vid == $vid && $tax) {
      $prSe = filter_province_by_region($tid);
      if(count($prSe) > 1) {
         drupal_set_message('Vùng miền chứa các danh mục con không thể xóa','error');
         $form_state['redirect'] = 'region';
         return;
      }
      taxonomy_term_delete($tid);
      drupal_set_message('Đã xóa vùng miền');
      $form_state['redirect'] = 'region';
    } else {
      drupal_set_message('Không tìm thấy vùng miền','error');
      $form_state['redirect'] = 'region';
    }
}

/*
* Validate the form region when user submit
*/
function adstrue_vendor_region_validate($form, &$form_state) {
  drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');
  $company_id = $form_state['values']['company_id'];
  if($company_id == 0) {
      form_set_error('company_id',t('Vui lòng chọn công ty'));
  }
  $name_region = $form_state['values']['name_region'];
  if($name_region == '') {
    form_set_error('Vui lòng nhập tên vùng miền');
  }
}

/*
* Submit the form region
*/
function adstrue_vendor_region_submit($form, &$form_state) {
  $tid = $form_state['values']['hidden_tid'];
  $name_region = $form_state['values']['name_region'];
  $description = $form_state['values']['region_description'];
  $company_id = $form_state['values']['company_id'];

  if(isset($tid) && $tid != '') {
    $term = taxonomy_term_load($tid);
    $term->name = $name_region;
    $term->description = $description;
    $term->field_region_company_id['und'][0]['tid'] = $company_id;
    $status = taxonomy_term_save($term);
    if($status== 2) {
      $form_state['redirect'] = 'region';
      drupal_set_message(t('Cập nhật vùng miền thành công'));
      return;
    } else {
      $form_state['redirect'] = 'region';
      drupal_set_message(t('Không thể cập nhật vùng miền'));
      return;
    }
  }
  $voca = taxonomy_vocabulary_machine_name_load('region');
  $vid = $voca->vid;
  //Create term object to add to the dabase
  $term = new stdClass;
  $term->name = $name_region;
  $term->description = $description;
  $term->field_region_company_id['und'][0]['tid'] = $company_id;
  $term->vid = $vid;
  taxonomy_term_save($term);
  if($term->tid) {
    $form_state['redirect'] = 'region';
    drupal_set_message('Đã tạo một vùng miền');
  } else {
    $form_state['redirect'] = 'region';
    drupal_set_message('Không thể tạo vùng miền');
  }

}
function region_adstrue_vendor($form, &$form_state) {
    global $user,$base_url;
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Vùng miền'),
        array('data' => 'Công ty'),
        array('data' => 'Thao tác'),
    );
    $index = 1;
    $voca = taxonomy_vocabulary_machine_name_load('region');
    $vid = $voca->vid;
    $terms = entity_load('taxonomy_term',FALSE,array('vid' => $vid));
    if(!isset($terms)) {
        $terms = array();
    }
    $rows = array();
    foreach($terms as $term) {
        if(isset($term->field_region_company_id['und'][0]['tid']) && !empty($term->field_region_company_id['und'][0]['tid'])) {
            $company_id = $term->field_region_company_id['und'][0]['tid'];
            $company = taxonomy_term_load($company_id);
        }

        // Them hang cho bang
        $rows[] = array(
            $index,
            $term->name,
            (isset($company) && $company) ? $company->name : '',
            '<div class="btn-group">
                <button type="button" class="btn btn-success">Thao tác</button>
                <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                    <span class="caret"></span>
                    <span class="sr-only">Toggle Dropdown</span>
                </button>
                <ul class="dropdown-menu" role="menu">
                    <li><a href="'.$base_url.'/region/edit/'.$term->tid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                    <li><a href="'.$base_url.'/region/delete/'.$term->tid.'/delete?destination=region" ><i class="fa fa-trash"></i> Xóa</a></li>
                </ul>
            </div>'
        );
        $index++;
    }

    $form = array();
    $form['danhsach_region'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div>'
    );
    $form['danhsach_region']['header'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/region/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Tạo vùng miền </a>',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['danhsach_region']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['danhsach_region']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}