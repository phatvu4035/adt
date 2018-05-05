<?php
/* 
* Building a form to update company
*/
function adstrue_vendor_add_company($form,&$form_state) {
	$form = build_form_company();
	return $form;
}

function adstrue_vendor_update_company($form,&$form_state,$value) {
	return build_form_company($value);
}

function build_form_company($tid = 0) {
	  drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');
    $voca = taxonomy_vocabulary_machine_name_load('company');
    $vid = $voca->vid;

    $term = taxonomy_term_load($tid);
    // No phai thuoc vocabulary la cong ty moi duoc hien thi tren form
    if($term) {
        if($term->vid !== $vid) {
          drupal_not_found();
        }
    } 

	$form['fieldset_info'] = array(
        '#type' => 'container',
        '#weight' => 3, 
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="wr">',
        '#suffix' => '</div>'
     );
    // Xay dung form
    $form['fieldset_info']['hidden_tid'] = array(
      '#type' => 'hidden',
      '#title' => t('id term'),
      '#default_value' => (isset($tid)) ? $tid : '',
      '#prefix' => '<div class="center_small_form">',
      '#suffix' => '</div>'
    );
    $form['fieldset_info']['header'] = array(
      '#type' => 'container',
      '#prefix' => '<div class="box-header with-border">',
      '#suffix' => '</div>'
    );

    $form['fieldset_info']['header']['header_title'] = array(
          '#type' => 'item',
          '#markup' => '<h3>Nhập thông tin công ty</h3>'
    );
    $form['fieldset_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['fieldset_info']['body']['name_company'] = array(
      '#type' => 'textfield',
      '#title' => t('Tên công ty'),
      '#default_value' => (isset($term->name)) ?  $term->name : '',
      '#prefix' => '<div class="center_small_form">',
      '#suffix' => '</div>',
      '#required' => TRUE,
    );
    $form['fieldset_info']['body']['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Mô tả'),
      '#default_value' => (isset($term->description)) ? $term->description : '',
      '#prefix' => '<div class="center_small_form">',
      '#suffix' => '</div>'
    );
    $form['fieldset_info']['footer'] = array(
      '#type' => 'container',
      '#prefix' => '<div class="box-footer with-border">',
        '#suffix' => '</div>'
    );
    $form['fieldset_info']['footer']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Lưu lại'),
      '#prefix' => '<div class="center_small_form"><div class="pull-right">',
      '#suffix' => '</div></div>',
      '#attributes' => array(
          'class' => array('btn','btn-primary')
      ),
    );
    $form['#submit'][] = 'adstrue_vendor_company_submit';
    $form['#validate'][] = 'adstrue_vendor_company_validate';
    return $form;
}

/*
* Delete company taxonomy form
*/
function delete_adstrue_vendor_company($form,&$form_state,$value) {
    $form = array();
    $form['header'] = array(
        '#type' => 'item',
        '#markup' => '<h4>Vui lòng xác nhận để xóa công ty</h4>',
    );
    $form['company_id'] = array(
        '#type' => 'hidden',
        '#value' => $value,
    );
    return confirm_form($form,'Vui lòng xác nhận để xóa vùng miền','company',t(''),t('Xác nhận'),t('Hủy bỏ'));
}

function delete_adstrue_vendor_company_submit($form,$form_state) {
    $tid = $form_state['values']['company_id'];
    $tax = taxonomy_term_load($tid);

    $voca = taxonomy_vocabulary_machine_name_load('company');
    $vid = $voca->vid;
    if($tax->vid == $vid && $tax) {
      $rgSe = filter_region_by_company($tid);
      
      if(count($rgSe) > 1) {
         drupal_set_message('Công ty chứa các danh mục con không thể xóa','error');
         return;
      }

      taxonomy_term_delete($tid);
      drupal_set_message('Đã xóa công ty');
      $form_state['redirect'] = 'company';
    } else {
      drupal_set_message('Không tìm thấy công ty','error');
      $form_state['redirect'] = 'company';
    }
}
/*
* Validate the form company when user submit
*/
function adstrue_vendor_company_validate($form, &$form_state) {
  drupal_add_css(drupal_get_path('module','adstrue_vendor').'/assets/css/custom_form.css');
  $name_company = $form_state['values']['name_company'];
  if($name_company == '') {
    form_set_error(t('Bạn vui lòng nhập tên công ty'));
  } 
}

/*
* Submit the form company
*/
function adstrue_vendor_company_submit($form, &$form_state) {
  $tid = $form_state['values']['hidden_tid'];
  $name_company = $form_state['values']['name_company'];
  $description = $form_state['values']['description'];
  if(isset($tid) && $tid) {
    $term = taxonomy_term_load($tid);
    $term->name = $name_company;
    $term->description = $description;
    $status = taxonomy_term_save($term);
    if($status== 2) {
        $form_state['redirect'] = 'company';
        drupal_set_message(t('Cập nhật công ty thành công')); 
    } else {
        $form_state['redirect'] = 'company';
        drupal_set_message(t('Không thể cập nhật công ty','error')); 
    }
    return;
  }
  // Create term object to add database
  $voca = taxonomy_vocabulary_machine_name_load('company');
  $vid = $voca->vid;
  $term = new stdClass();
  $term->name = $name_company;
  $term->description = $description;
  $term->vid = $vid;
  taxonomy_term_save($term);
  if($term->tid) {
      $form_state['redirect'] = 'company';
      drupal_set_message(t('Tạo công ty thành công'));
  } else {
      $form_state['redirect'] = 'company';
      drupal_set_message(t('Không thể tạo công ty','error'));
  }
}

function  company_adstrue_vendor($form, &$form_state){
   global $user,$base_url;
   $form = array();
   $header = array(
      array('data' => 'STT'),
      array('data' => 'Tên công ty'),
      array('data' => 'Ghi chú'),
      array('data' => 'Thao tác'),
   );

    $index = 1;
    $voca = taxonomy_vocabulary_machine_name_load('company');
    $vid = $voca->vid;
    $terms = entity_load('taxonomy_term',FALSE,array('vid' => $vid));
    if(!isset($terms)) {
        $terms = array();
    }
    $rows = array();
    foreach($terms as $term) {
        // Them du lieu cho tung hang
        $rows[] = array(
              $index,
              $term->name,
              $term->description,
              '<div class="btn-group">
                  <button type="button" class="btn btn-success">Thao tác</button>
                  <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                      <span class="caret"></span>
                      <span class="sr-only">Toggle Dropdown</span>
                  </button>
                  <ul class="dropdown-menu" role="menu">
                      <li><a href="'.$base_url.'/company/edit/'.$term->tid.'" ><i class="fa fa-edit"></i> Sửa</a></li>
                      <li><a href="'.$base_url.'/company/delete/'.$term->tid.'/delete?destination=company" ><i class="fa fa-trash"></i> Xóa</a></li>
                  </ul>
              </div>'
        );
        $index++;
    }
    
    $form = array();
    $form['company_danhsach'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success','wrapper-form')
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div>'
    );
    $form['company_danhsach']['header'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/company/add" class="btn btn-success pull-right"><i class="fa fa-plus"></i> Tạo công ty </a>',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['company_danhsach']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['company_danhsach']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}