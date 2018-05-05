<?php

/*
* Xay dung lich phat song
*/
function control_podcast_form($form,&$form_state) {
  drupal_add_css(drupal_get_path('module','adstrue_podcast').'/assets/css/broadcast.css');
   $limit = 10;
   $query = db_select('node','n')->extend('PagerDefault')->distinct()->limit($limit);
   $query->condition('n.type','playlist');
   $query->fields('n',array('nid'));
   $query->orderBy('n.changed');
   $result = $query->execute();
   $form = array();
   $form = build_form_body_broadcast($result);
   return $form;
    
}

function edit_broadcast_by_term($form,&$form_state) {
    if(isset($_GET['company']) && (int)$_GET['company']) {
      $company_id = $_GET['company'];
    } else {
      $company_id = 0;
    }
    if(isset($_GET['region']) && (int)$_GET['region']) {
      $region_id = $_GET['region'];
    } else {
      $region_id = 0;
    }

    if(isset($_GET['province']) && (int)$_GET['province']) {
      $province_id = $_GET['province'];
    } else {
      $province_id = 0;
    }

    if(isset($_GET['store']) && (int)$_GET['store']) {
      $store_id = $_GET['store'];
    } else {
      $store_id = 0;
    }

}


function build_form_body_broadcast($result) {
    // Load all company for very first edit
    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;
    // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }

    $default_company_id = key($cSelect);
    $rgSe  = filter_region_by_company($default_company_id);

    if(isset($_GET['company']) && (int)$_GET['company'] > 0) {
       $default_company_id = $_GET['company'];
       $rgSe  = filter_region_by_company($default_company_id);
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


  $form['podcast_property'] = array(
      '#type' => 'container',
      '#weight' => 2,
      '#attributes' => array(
          'class' => array('box','box-success')
      ),
      '#prefix' => '<div class="col-md-12 wrPl">',
      '#suffix' => '</div>'
    );

    $form['podcast_name'] = array(
       '#type' => 'textfield', 
       '#title' => t('Tên lịch phát sóng'), 
       '#maxlength' => 60,
       '#required' => TRUE,
    );
    $form['podcast_property']['company_id'] = array(
        '#type' => 'select',
        '#title' => t('Tên công ty'),
        '#options' => $cSelect,
        '#ajax' => array(
          'callback' => 'broadcast_load_region_by_company',
          'method' =>'replace',
          'wrapper' => 'region-wrapper',
        ),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#default_value' => $default_company_id,
    );
    $form['podcast_property']['region_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div id="region-wrapper">',
        '#title' => t('Chọn vùng miền'),
        '#options' => array(0 => t('Chọn')) + $rgSe,
        '#ajax' => array(
          'callback' => 'broadcast_load_province_by_region',
          'method' =>  'replace',
          'wrapper' => 'province-wrapper',
        ),
        '#default_value' => (isset($region_id)) ? $region_id : 0,
        '#suffix' => '</div>',
        '#validated' => TRUE
    );
    $form['podcast_property']['province_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div id="province-wrapper">',
        '#title' => t('Chọn tỉnh thành'),
        '#options' => (isset($prSe)) ? $prSe : array(0 => t('Chọn')),
        '#ajax' => array(
          'callback' => 'broadcast_load_store_by_province',
          'method' =>  'replace',
          'wrapper' => 'store-wrapper',
        ),
          '#default_value' =>(isset($province_id)) ? $province_id  : 0,
          '#suffix' => '</div>',
          '#validated' => TRUE
    );

    $form['podcast_property']['store_id'] = array(
        '#type' => 'select',
        '#prefix' => '<div id="store-wrapper">',
        '#title' => t('Chọn cửa hàng'),
        '#options' =>  (isset($stSe)) ? $stSe : array(0 => t('Chọn')),
        '#default_value' =>(isset($store_id)) ? $store_id : 0,
        '#suffix' => '</div>',
        '#validated' => TRUE
    ); 

    $form['podcast_grid_playlist'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
        '#prefix' => '<div class="grid_playlist">',
        '#suffix' => '</div>'
    );

    $header_playlist = '<div class="col-md-1 stt">
        <p>STT</p>
      </div>
      <div class="col-md-3 playlist">
        <p>Tên playlist</p>
      </div>
      <div class="col-md-3 ngaybatdau">
          Ngày bắt đầu
      </div>
      <div class="col-md-3 ngaykethuc">
          Ngày kết thúc
      </div>
      <div class="col-md-1 chonplaylist">
         Chọn
      </div>
    ';

    $form['podcast_grid_playlist']['header'] = array(
        '#type' => 'item',
        '#markup' =>$header_playlist,
        '#prefix' => '<div class="box-header with-border header_add_pod">',
        '#suffix' => '</div>'
    );

    $form['countplaylist'] = array(
        '#type' => 'hidden',
        '#value' => $result->rowCount(),
    );
    if($result->rowCount()) {
        $index = 1;
        foreach ($result as $row) {
            $playlist = node_load($row->nid);
            $form['podcast_grid_playlist']['body']['row'.$index] = array(
                '#type' => 'container',
                '#weight' => 2,
                '#attributes' => array(
                    'class' => array('box','box-success')
                ),
                '#prefix' => '<div class="playlist_rec">',
                '#suffix' => '</div>'
            );
            $form['podcast_grid_playlist']['body']['row'.$index]['stt'.$index] = array(
                '#type' => 'item',
                '#markup' => $index,
                '#prefix' => '<div class="col-md-1">',
                '#suffix' => '</div>'
             );
             $form['podcast_grid_playlist']['body']['row'.$index]['playlist_id'.$index] = array(
                '#type' => 'hidden',
                '#value' => $playlist->nid,
             );
             $form['podcast_grid_playlist']['body']['row'.$index]['playlist_name'.$index] = array(
                '#type' => 'item',
                '#markup' => '<a href="">'.$playlist->title.'</a>',
                '#prefix' => '<div class="col-md-3">',
                '#suffix' => '</div>'
             );
             $form['podcast_grid_playlist']['body']['row'.$index]['ngaybatdau'.$index] = array(
                 '#type' => 'date_popup',
                  '#timepicker' => 'timepicker',
                  '#date_label_position' => 'none',
                  '#timepicker_options' => array(
                        'rows' => 6,
                        'minutes' => array(
                          'starts' => 0,
                          'ends' => 56,
                          'interval' => 4,
                        ),
                        'showCloseButton' => TRUE,
                        'closeButtonText' => t('Close'),
                      ),
                  '#date_format' => 'd/m/Y - H:i',

                  '#prefix' => '<div class="col-md-3" class="nbd_cell">',
                  '#suffix' => '</div>',
             );
             $form['podcast_grid_playlist']['body']['row'.$index]['ngayketthuc'.$index]  = array(
                  '#type' => 'date_popup',
                  '#timepicker' => 'timepicker',
                  '#date_label_position' => 'none',
                  '#timepicker_options' => array(
                        'rows' => 6,
                        'minutes' => array(
                          'starts' => 0,
                          'ends' => 56,
                          'interval' => 4,
                        ),
                        'showCloseButton' => TRUE,
                        'closeButtonText' => t('Close'),
                      ),
                  '#date_format' => 'd/m/Y - H:i',

                  '#prefix' => '<div class="col-md-3" class="nkt_cel">',
                  '#suffix' => '</div>',
              );
             $form['podcast_grid_playlist']['body']['row'.$index]['chon'.$index] = array(
                  '#type' =>'checkbox',
                  '#default_value' => 0,
                  '#prefix' => '<div class="col-md-1">',
                  '#suffix' => '</div>',
             );
             $index++;
        }
    }

    $form['podcast_property']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );

    $form['podcast_additional']['submit'] = array(
      '#type' => 'submit',
      '#weight' => 6,
      '#prefix' => '<div>',
      '#value' => t('Cập nhật'),
      '#suffix' => '</div>',
    );
    return $form;
}


/*
* AJAX de load cac khu vuc dua tren cong ty
*/
function broadcast_load_region_by_company($form,$form_state) {
   $matches = array();
   $company_id = $form_state['values']['company_id'];
   $tids = array();
   // Get taxonomy by field value
    $tids = db_select('field_data_field_region_company_id','rbycom')
             ->fields('rbycom',array('entity_id'))
             ->condition('field_region_company_id_tid',$company_id,'=')
             ->execute()
             ->fetchCol();
    $terms = taxonomy_term_load_multiple($tids);

  foreach($terms as $term) {
    $matches[$term->tid] = $term->name;
  }
  if(count($matches) == 0) {
    $matches = array(0 => t('Chọn'));
  }
    $form['podcast_property']['region_id']['#options'] = array(0=>t('Chọn')) + $matches;
    return $form['podcast_property']['region_id'];
}

/*
* AJAX de load tinh thanh theo khu vuc
*/
function broadcast_load_province_by_region($form,$form_state) {
    $matches = array();
    $region_id = $form_state['values']['region_id'];
    // Get taxonomy by field value
      $tids = array();
     // Get taxonomy by field value
      $tids = db_select('field_data_field_region_provice_id','pbyrg')
               ->fields('pbyrg',array('entity_id'))
               ->condition('field_region_provice_id_tid',$region_id,'=')
               ->execute()
               ->fetchCol();
      $terms = taxonomy_term_load_multiple($tids);
    foreach($terms as $term) {
      $matches[$term->tid] = $term->name;
    }

    if(count($matches) == 0) {
      $matches = array(0 => t('Chọn'));
    }

  $form['podcast_property']['province_id']['#options'] = array(0=>t('Chọn')) + $matches;
  return $form['podcast_property']['province_id'];
}

/*
* Ajax de load cac cua hang dua tren tinh thanh
*/
function broadcast_load_store_by_province($form,&$form_state) {
    $matches = array();
    $province_id = $form_state['values']['province_id'];
    $tids = array();
    // Get taxonomy by field value
      $tids = db_select('field_data_field_province_store_id','stbypr')
               ->fields('stbypr',array('entity_id'))
               ->condition('field_province_store_id_tid',$province_id,'=')
               ->execute()
               ->fetchCol();
      $terms = taxonomy_term_load_multiple($tids);
    foreach($terms as $term) {
      $matches[$term->tid] = $term->name;
    }
    if(count($matches) == 0) {
      $matches = array(0 => t('Chọn'));
    }

    $form['podcast_property']['store_id']['#options'] = array(0=>t('Chọn')) + $matches;
    return $form['podcast_property']['store_id'];
}


// Load string suggestion
function load_playlist_suggestion_job($search_string = '') {
  $matches = array();

    $result = db_select('node','n')
            ->fields('n')
            ->condition('title', '%'.db_like($playlist_string).'%','LIKE')
            ->condition('type','playlist','=')
            ->execute()
            ->fetchAll();
    foreach($result as $playlist) {
        $matches[$playlist->title.'|'.$playlist->nid] = $playlist->title.'|'.$playlist->nid;
    }

    drupal_json_output($matches);
}
function control_podcast_form_validate($form,&$form_state) {
  drupal_add_css(drupal_get_path('module','adstrue_podcast').'/assets/css/broadcast.css');

    $values  = $form_state['values'];
    $keys = array_keys($values,1);

    $title = $form_state['values']['podcast_name'];
    if(strrpos($title,'|') !== FALSE) {
       form_set_error('podcast_name','Tên lịch phát sóng không được chứa ký tự "|"');
    }

    foreach($keys as $key) {
      if(strrpos($key, 'chon') !== FALSE) {
          $i = substr($key,4);
          if($form_state['values']['ngaybatdau'.$i] !== '' && $form_state['values']['ngaybatdau'.$i]) {
              $startime = DateTime::createFromFormat('Y-m-d H:i',$form_state['values']['ngaybatdau'.$i]);
              $startU = $startime->getTimestamp();
           } else {
               $startU = '';
           }
           if($form_state['values']['ngayketthuc'.$i] !== '' && $form_state['values']['ngayketthuc'.$i]) {
               $endate_obj = DateTime::createFromFormat('Y-m-d H:i',$form_state['values']['ngayketthuc'.$i]);
               $endU = (isset($endate_obj)) ? $endate_obj->getTimestamp() : 0;
           } else {
               $endU = '';
           }

          if($startU == '') {
              form_set_error('ngaybatdau'.$i,'Vui lòng nhập thời gian bắt đầu');
          }
          if($endU == '') {
              form_set_error('ngayketthuc'.$i,'Vui lòng nhập thời gian kết thúc');
          }
          if($startU > $endU && $endU) {
              form_set_error('ngaybatdau'.$i,'Thời gian kết thúc phải lớn hơn thời gian bắt đầu');
          }

      }
    }
}


// //submit function
function control_podcast_form_submit($form,&$form_state) {
    global $user;
    drupal_add_css(drupal_get_path('module','adstrue_podcast').'/assets/css/broadcast.css');

    $title = $form_state['values']['podcast_name'];
    $store_id = $form_state['values']['store_id'];
    $couts = $form_state['values']['countplaylist'];
    $province_id = $form_state['values']['province_id'];
    $region_id = $form_state['values']['region_id'];
    $company_id = $form_state['values']['company_id'];

    $query = db_select('taxonomy_term_data', 'ttd');
    $query->fields('ttd', array('tid','name'));
    if($company_id && $region_id && $province_id && $store_id){
        $query->leftJoin('field_data_field_mac_store_id','msi','msi.entity_id=ttd.tid');
        $query->leftJoin('field_data_field_mac_province_id','pri','pri.entity_id=ttd.tid');
        $query->leftJoin('field_data_field_mac_region_id','rgi','rgi.entity_id=ttd.tid');
        $query->leftJoin('field_data_field_mac_company_id','cpi','cpi.entity_id=ttd.tid');
        $query->condition('msi.field_mac_store_id_tid',$store_id);
        $query->condition('cpi.field_mac_company_id_tid',$company_id);
        $query->condition('rgi.field_mac_region_id_tid',$region_id);
        $query->condition('pri.field_mac_province_id_tid',$province_id);

    }elseif($company_id && $region_id && $province_id){
        $query->leftJoin('field_data_field_mac_company_id','cpi','cpi.entity_id=ttd.tid');
        $query->condition('cpi.field_mac_company_id_tid',$company_id);
        $query->leftJoin('field_data_field_mac_region_id','rgi','rgi.entity_id=ttd.tid');
        $query->condition('rgi.field_mac_region_id_tid',$region_id);
        $query->leftJoin('field_data_field_mac_province_id','pri','pri.entity_id=ttd.tid');
        $query->condition('pri.field_mac_province_id_tid',$province_id);
    }elseif ($company_id && $region_id){
        $query->leftJoin('field_data_field_mac_company_id','cpi','cpi.entity_id=ttd.tid');
        $query->condition('cpi.field_mac_company_id_tid',$company_id);
        $query->leftJoin('field_data_field_mac_region_id','rgi','rgi.entity_id=ttd.tid');
        $query->condition('rgi.field_mac_region_id_tid',$region_id);

    }elseif ($company_id){
        $query->leftJoin('field_data_field_mac_company_id','cpi','cpi.entity_id=ttd.tid');
        $query->condition('cpi.field_mac_company_id_tid',$company_id);
    } 

    $result = $query->execute();

    $values  = $form_state['values'];
    $keys = array_keys($values,1);

    // Lấy toàn bộ các Mac query được cho vào 1 mảng
    if($result->rowCount()) {
      $mac_array = array();
      foreach ($result as $row) {
        $mac_array[] = array(
          'tid' => $row->tid,
          'name' => $row->name
        );
      }
    }

    if($result->rowCount()) {
        $pass = 0;
          foreach($keys as $key) {

            if(strrpos($key, 'chon') !== FALSE) {

                $playlist_broadcast = new stdClass();
                $playlist_broadcast->title = $title;
                $playlist_broadcast->type = 'broadcast_playlist';
                $playlist_broadcast->language = LANGUAGE_NONE;
                $playlist_broadcast->uid = $user->uid;
                $playlist_broadcast->field_company_id_playlist[LANGUAGE_NONE][0]['tid'] = $company_id;
                $playlist_broadcast->field_region_id_playlist[LANGUAGE_NONE][0]['tid'] = $region_id;
                $playlist_broadcast->field_province_id_playlist[LANGUAGE_NONE][0]['tid'] = $province_id;
                $playlist_broadcast->field_store_id_playlist[LANGUAGE_NONE][0]['tid'] = $store_id;

                // Lay vi tri cua tung $i
                $i = substr($key,4);
                if($form_state['values']['ngaybatdau'.$i] !== '' && $form_state['values']['ngaybatdau'.$i]) {
                    $startime = DateTime::createFromFormat('Y-m-d H:i',$form_state['values']['ngaybatdau'.$i]);
                    $startU = $startime->getTimestamp();
                 } else {
                     $startU = '';
                 }
                 if($form_state['values']['ngayketthuc'.$i] !== '' && $form_state['values']['ngayketthuc'.$i]) {
                     $endate_obj = DateTime::createFromFormat('Y-m-d H:i',$form_state['values']['ngayketthuc'.$i]);
                     $endU = (isset($endate_obj)) ? $endate_obj->getTimestamp() : 0;
                 } else {
                     $endU = '';
                 }

                 // Lây playlist ì từ danh sách chọn
                 $playlist_id = $form_state['values']['playlist_id'.$i];

                 // Them vaoban ghi 
                 $playlist_broadcast->field_broadcast_playlist_id[LANGUAGE_NONE][0]['nid'] = $playlist_id;
                 $playlist_broadcast->field_start_time_playlist[LANGUAGE_NONE][0]['value'] = $startU;
                 $playlist_broadcast->field_end_time_playlist[LANGUAGE_NONE][0]['value'] = $endU;

                  // Load qua tung mac
                  foreach ($mac_array as $mac) {
                     $node = new stdClass();
                     $node->title = $title .'|'.date('d-m-y H:i:s',$startU).'|'.date('d-m-y H:i:s',$endU);
                     $node->language = LANGUAGE_NONE;
                     $node->type = 'broadcast_playlist_id';
                     $node->field_broadcast_company_id[$node->language][] = array('tid' => $company_id);
                     $node->field_broadcast_region_id[$node->language][] = array('tid' => $region_id);
                     $node->field_broadcast_province_id[$node->language][] = array('tid' => $province_id);
                     $node->field_broadcast_store_id[$node->language][] = array('tid' => $store_id);
                     
                     // Them mac ip vao node ngoai broadcast playlist 
                     $playlist_broadcast->field_mac_id_playlist[LANGUAGE_NONE][] = array('tid' => $mac['tid']);

                     $node = interact_new_broadcast_node($node,$mac['name'],$playlist_id,$company_id,$region_id,$province_id,$store_id,$startU,$endU);
                     if($node->nid) {
                        $pass++;
                     }
                  }

                  node_save($playlist_broadcast);
            }
          }

        if($pass >= $result->rowCount()) {
            drupal_set_message('Tạo thành công lịch phát sóng');
            $form_state['redirect'] = 'podcast/list';

        } else {
           drupal_set_message('Có lỗi trong quá trình tạo lịch phát sóng','error');
           $form_state['redirect'] = 'podcast/list';
        }
    }

}


function interact_new_broadcast_node($node,$mac,$playlist_id = '',$company_id = '',$region_id = '',$province_id = '',$store_id = '',$startime= '',$endtime = '') {
      global $user;
      $mac = trim($mac);
      $node->uid = $user->uid;
      $node->field_podcast_playlist_id[$node->language][0]['nid'] = $playlist_id;
      $node->field_broadcast_mac_address[$node->language][0]['value'] = $mac;
      if($startime) {
          $node->field_broadcast_date_toplay[$node->language][] = array(
            'value' => $startime,
            'timezone' => 'Asia/Ho_Chi_Minh',
            'timezone_db' => 'UTC',
            'date_type' => 'datetime',
        );
      }
      if($endtime) {
          $node->field_broadcast_endplay[$node->language][] = array(
              'value' => $endtime,
              'timezone' => 'Asia/Ho_Chi_Minh',
              'timezone_db' => 'UTC',
              'date_type' => 'datetime',
          );
      }
      node_save($node);
      return $node;
}

/*
* build table
*/
function list_podcast_manage_all($form, &$form_state) {
  $header = array(
    array('data' => 'STT'),
    array('data' => 'Tên lịch phát sóng'),
    array('data' => 'Tên playlist'),
    array('data' => 'Công ty'),
    array('data' => 'Vùng miền'),
    array('data' => 'Tỉnh thành'),
    array('data' => 'Cửa hàng'),
    array('data' => 'Thời gian bắt đầu'),
    array('data' => 'Thời gian kết thúc'),
    array('data' => 'Thao tác'),
  );
     // Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
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

        $prSe  = filter_province_by_region($region_id);
    }
    if(isset($_GET['province']) && (int)$_GET['province'] > 0) {
        $company_id = $_GET['company'];
        $region_id = $_GET['region'];
        $province_id = $_GET['province'];
        $rgSe = filter_region_by_company($company_id);
        $prSe  = filter_province_by_region($region_id);
        $stSe = filter_store_by_province($prSe);
    }
    if(isset($_GET['store']) && (int)$_GET['store'] > 0) {
        $company_id = $_GET['company'];
        $region_id = $_GET['region'];
        $province_id = $_GET['province'];
        $store_id = $_GET['store'];
        $rgSe = filter_region_by_company($company_id);
        $prSe  = filter_province_by_region($region_id);
        $stSe = filter_store_by_province($prSe);
    }

    if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
        $keyword = $_GET['keyword'];
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    
  $rows = playlist_broadcast_table();

  $form = array();
    $form['lichphatsong'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success','wrapper-form')
        ),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $form['lichphatsong']['header_filter'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-header')
        ),
        '#prefix' => '<div class="col-md-12 box-header with-border">',
        '#suffix' => '</div>'
    );
     
    $form['lichphatsong']['header_filter']['company'] = array(
        '#type' => 'select',
         '#title' => t('Công ty'),
         '#options' => $cSelect,
         '#ajax' => array(
            'callback' => 'epg_load_region_by_company_table',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
          ),
         '#prefix' => '<div class="col-md-3 box-search-item">',
         '#suffix' => '</div>',
         '#default_value' => isset($company_id) ? $company_id : 0,
    );

    $form['lichphatsong']['header_filter']['region'] = array(
        '#type' => 'select',
         '#title' => t('Lọc theo khu vực'),
         '#options' => (isset($rgSe)) ? $rgSe : array(0 => t('Chọn vùng miền')),
         '#ajax' => array(
            'callback' => 'epg_load_province_by_region_table',
            'method' =>  'replace',
            'wrapper' => 'province-wrapper',
          ),
         '#default_value' => isset($region_id) ? $region_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="region-wrapper">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE,
    );
    $form['lichphatsong']['header_filter']['province'] = array(
        '#type' => 'select',
         '#title' => t('Lọc theo tỉnh thành'),
         '#options' => isset($prSe) ? $prSe : array(0 => t('Chọn tỉnh thành')),
         '#ajax' => array(
            'callback' => 'epg_load_store_by_province_table',
            'method' =>  'replace',
            'wrapper' => 'store-wrapper',
          ),
         '#default_value' => isset($province_id) ? $province_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="province-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE,
    );
    
    $form['lichphatsong']['header_filter']['store'] = array(
        '#type' => 'select',
         '#title' => t('Lọc theo cửa hàng'),
         '#options' => isset($stSe) ? $stSe : array(0 => t('Chọn cửa hàng')),
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="store-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#default_value' => isset($store_id) ? $store_id : 0,
         '#validated' => TRUE,
    );

    $form['lichphatsong']['header_filter']['keyword'] = array(
        '#type' => 'textfield',
        '#title' => t('Tên playlist'),
        '#default_value' => isset($keyword) ? $keyword : NULL,
        '#attributes' => array(
          'placeholder' => 'Tên playlist',
        ),
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-6 box-search-item">',
        '#suffix' => '</div>',
    );

    $form['lichphatsong']['header_filter']['submit_search'] = array(
      '#type' => 'submit',
      '#value' => t('Tìm kiếm'),
      '#prefix' => '<div class="col-md-2">',
        '#suffix' => '</div>',
    );
    $form['lichphatsong']['header_filter']['create_broadcast'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$GLOBALS['base_url'].'/podcast/add" target="_blank" class="btn btn-success"><i class="fa fa-plus"></i> Thêm lịch phát sóng </a>',
        '#prefix' => '<div class="col-md-4 box-search-item">',
        '#suffix' => '</div>',
    );
   

    $form['lichphatsong']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['lichphatsong']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    $form['#method'] = 'GET';
    return $form;
}

function delete_broadcast_by_term($form,&$form_state,$value) {
    $form = array();
    $form['header'] = array(
      '#type' => 'item',
      '#markup' => '<h4>Vui lòng xác nhận để xóa lịch phát sóng</h4>'
    );
    $node = node_load($value);
    $form['node_broadcast_playlist'] = array(
      '#type' => 'hidden',
      '#value' => $value,
    );
    $active = array(1 => t('Chỉ xóa khỏi địa chỉ MAC này'), 2 => t('Xóa hết'));

    $form['the_id'] = array(
        '#type' => 'value',
        '#value' => 1
    );
    return confirm_form($form,'Vui lòng xác nhận để xóa lịch phát sóng','podcast/list',t(''),t('Xác nhận'),t('Hủy bỏ'));
}
function delete_broadcast_by_term_submit($form,&$form_state) {
  if($form_state['values']['the_id'] == 1) {
      
      $node_id = $form_state['values']['node_broadcast_playlist'];
      $node = node_load($node_id);

      // Lay thong tin can thiet
       $company_id = isset($node->field_company_id_playlist[$node->language][0]['tid']) ? $node->field_company_id_playlist[$node->language][0]['tid'] : 0;
        $company = taxonomy_term_load($company_id);
        $region_id = isset($node->field_region_id_playlist[$node->language][0]['tid']) ? $node->field_region_id_playlist[$node->language][0]['tid'] : 0;
        $region  = taxonomy_term_load($region_id);
        $province_id = isset($node->field_province_id_playlist[$node->language][0]['tid']) ? $node->field_province_id_playlist[$node->language][0]['tid'] : 0;
        $province = taxonomy_term_load($province_id);
        $store_id = isset($node->field_store_id_playlist[$node->language][0]['tid']) ? $node->field_store_id_playlist[$node->language][0]['tid'] : 0;
        $store = taxonomy_term_load($store_id);

        $playlist_id = isset($node->field_broadcast_playlist_id[$node->language][0]['nid']) ? $node->field_broadcast_playlist_id[$node->language][0]['nid'] : 0;
        $playlist = node_load($playlist_id);

        $sPlay = isset($node->field_start_time_playlist[$node->language][0]['value']) ? $node->field_start_time_playlist[$node->language][0]['value'] : 0;
        $ePlay = isset($node->field_end_time_playlist[$node->language][0]['value']) ? $node->field_end_time_playlist[$node->language][0]['value'] : 0;

        node_delete($node_id);

        $query = db_select('node','n');
        $query->innerJoin('field_data_field_broadcast_company_id','cpi','cpi.entity_id = n.nid');
        $query->innerJoin('field_data_field_broadcast_region_id','rgi','rgi.entity_id = n.nid');
        $query->innerJoin('field_data_field_broadcast_province_id','pri','pri.entity_id = n.nid');
        $query->innerJoin('field_data_field_broadcast_store_id','sti','sti.entity_id = n.nid');
        $query->innerJoin('field_data_field_podcast_playlist_id','pli','pli.entity_id = n.nid');
        $query->innerJoin('field_data_field_broadcast_date_toplay','sttime','sttime.entity_id = n.nid');
        $query->innerJoin('field_data_field_broadcast_endplay','entime','entime.entity_id = n.nid');
        $query->condition('n.type','broadcast_playlist_id');
        $query->condition('pli.field_podcast_playlist_id_nid',$playlist_id);
        $query->condition('cpi.field_broadcast_company_id_tid',$company_id);
        $query->condition('rgi.field_broadcast_region_id_tid',$region_id);
        $query->condition('pri.field_broadcast_province_id_tid',$province_id);
        $query->condition('sti.field_broadcast_store_id_tid',$store_id);
        $query->condition('sttime.field_broadcast_date_toplay_value',$sPlay);
        $query->condition('entime.field_broadcast_endplay_value',$ePlay);
        $query->fields('n',array('nid'));
        $result = $query->execute();
        $nids = $result->fetchCol();

        node_delete_multiple($nids);

        drupal_set_message('Lịch phát sóng ' .$title. 'đã được xóa');
        $form_state['redirect'] = 'podcast/list';
  }
}

function epg_load_region_by_company_table($form,$form_state) {
     $matches = array();
     $company_id = $form_state['values']['company'];
     $tids = array();
     // Get taxonomy by field value
      $tids = db_select('field_data_field_region_company_id','rbycom')
               ->fields('rbycom',array('entity_id'))
               ->condition('field_region_company_id_tid',$company_id,'=')
               ->execute()
               ->fetchCol();
      $terms = taxonomy_term_load_multiple($tids);

    foreach($terms as $term) {
      $matches[$term->tid] = $term->name;
    }
    if(count($matches) == 0) {
      $matches = array(0 => t('Chọn'));
    }
    
    $form['lichphatsong']['header_filter']['region']['#options'] = array(0=>t('Chọn khu vực')) + $matches;
    $form['lichphatsong']['header_filter']['region']['#prefix'] = '<div id="region-wrapper">';
    return $form['lichphatsong']['header_filter']['region'];
}

function epg_load_province_by_region_table($form,$form_state) {
      $matches = array();
      $region_id = $form_state['values']['region'];
      // Get taxonomy by field value
      $tids = array();
     // Get taxonomy by field value
      $tids = db_select('field_data_field_region_provice_id','pbyrg')
               ->fields('pbyrg',array('entity_id'))
               ->condition('field_region_provice_id_tid',$region_id,'=')
               ->execute()
               ->fetchCol();
      $terms = taxonomy_term_load_multiple($tids);
      foreach($terms as $term) {
        $matches[$term->tid] = $term->name;
      }
      if(count($matches) == 0) {
        $matches = array(0 => t('Chọn'));
      }
      $form['lichphatsong']['header_filter']['province']['#options'] = array(0=>t('Chọn')) + $matches;
      $form['lichphatsong']['header_filter']['province']['#prefix'] = '<div id="province-wrapper">';
      return $form['lichphatsong']['header_filter']['province'];
}


function epg_load_store_by_province_table($form,$form_state) {
    $matches = array();
    $province_id = $form_state['values']['province'];
    $tids = array();
    // Get taxonomy by field value
      $tids = db_select('field_data_field_province_store_id','stbypr')
               ->fields('stbypr',array('entity_id'))
               ->condition('field_province_store_id_tid',$province_id,'=')
               ->execute()
               ->fetchCol();
      $terms = taxonomy_term_load_multiple($tids);
    foreach($terms as $term) {
      $matches[$term->tid] = $term->name;
    }
    if(count($matches) == 0) {
      $matches = array(0 => t('Chọn'));
    }


    $form['lichphatsong']['header_filter']['store']['#options'] = array(0=>t('Chọn')) + $matches;
    $form['lichphatsong']['header_filter']['store']['#prefix'] = '<div id="store-wrapper">';
    return $form['lichphatsong']['header_filter']['store'];
}


function playlist_broadcast_table() {
  $limit = 50;
  $query = db_select('node','n')->extend('PagerDefault')->distinct()->limit($limit);
  $query->innerJoin('field_data_field_company_id_playlist','cpi','cpi.entity_id = n.nid');
  $query->innerJoin('field_data_field_region_id_playlist','rgi','rgi.entity_id = n.nid');
  $query->innerJoin('field_data_field_province_id_playlist','pri','pri.entity_id = n.nid');
  $query->innerJoin('field_data_field_store_id_playlist','sti','sti.entity_id = n.nid');
  $query->innerJoin('field_data_field_broadcast_playlist_id','pli','pli.entity_id = n.nid');
  $query->condition('n.type','broadcast_playlist');
  $notStr = TRUE;

  if(isset($_GET['keyword']) && !empty($_GET['keyword'])) {
       $keyword = $_GET['keyword'];
       $keyword = trim($keyword);
       $query->condition('n.title','%'.db_like($keyword).'%','LIKE');
       $notStr = FALSE;
  }

  if(isset($_GET['company']) && (int)$_GET['company'] > 0 && $notStr)  {
      $company_id = $_GET['company'];
      // $query->leftJoin('field_data_field_broadcast_company_id','cpi','cpi.entity_id = n.nid');
      $query->condition('cpi.field_company_id_playlist_tid',$company_id);
  }
  if(isset($_GET['region']) && (int)$_GET['region'] > 0 && $notStr)  {
      $region_id = $_GET['region'];
      // $query->leftJoin('field_data_field_broadcast_region_id','rpi','rpi.entity_id = n.nid');
      $query->condition('rgi.field_region_id_playlist_tid',$region_id);
  }
  if(isset($_GET['province']) && (int)$_GET['province'] > 0 && $notStr)  {
      $province_id = $_GET['province'];
      // $query->leftJoin('field_data_field_broadcast_province_id','prpi','prpi.entity_id = n.nid');
      $query->condition('pri.field_province_id_playlist_tid',$province_id);
  }
  if(isset($_GET['store']) && (int)$_GET['store'] > 0 && $notStr)  {
      $store_id = $_GET['store'];
      // $query->leftJoin('field_data_field_broadcast_store_id','sti','sti.entity_id = n.nid');
      $query->condition('sti.field_store_id_playlist_tid',$store_id);
  }
  $query->fields('n',array('title','nid'));
  $query->orderBy('n.created','DESC');
  $result = $query->execute();
  if($result->rowCount()) {
    $index = 1;
    foreach($result as $row) {
       $node = node_load($row->nid);
        $company_id = isset($node->field_company_id_playlist[$node->language][0]['tid']) ? $node->field_company_id_playlist[$node->language][0]['tid'] : 0;
        $company = taxonomy_term_load($company_id);
        $region_id = isset($node->field_region_id_playlist[$node->language][0]['tid']) ? $node->field_region_id_playlist[$node->language][0]['tid'] : 0;
        $region  = taxonomy_term_load($region_id);
        $province_id = isset($node->field_province_id_playlist[$node->language][0]['tid']) ? $node->field_province_id_playlist[$node->language][0]['tid'] : 0;
        $province = taxonomy_term_load($province_id);
        $store_id = isset($node->field_store_id_playlist[$node->language][0]['tid']) ? $node->field_store_id_playlist[$node->language][0]['tid'] : 0;
        $store = taxonomy_term_load($store_id);

        $playlist_id = isset($node->field_broadcast_playlist_id[$node->language][0]['nid']) ? $node->field_broadcast_playlist_id[$node->language][0]['nid'] : 0;
        $playlist = node_load($playlist_id);

        $sPlay = isset($node->field_start_time_playlist[$node->language][0]['value']) ? $node->field_start_time_playlist[$node->language][0]['value'] : 0;
        $ePlay = isset($node->field_end_time_playlist[$node->language][0]['value']) ? $node->field_end_time_playlist[$node->language][0]['value'] : 0;
        

        $rows[] = array(
          $index,
          $row->title,
          (isset($playlist) && $playlist) ? $playlist->title : '',
          (isset($company) && $company) ? $company->name : '',
          (isset($region) && $region) ? $region->name : '',
          (isset($province) && $province) ? $province->name : '',
          (isset($store) && $store) ? $store->name : '',
          date('H:i:s m-d-y',$sPlay),
          date('H:i:s m-d-y',$ePlay),
          '<ul class="tools">    
              <li><a href="'.$GLOBALS['base_url'].'/podcast/'.$row->nid.'/delete?destination=podcast/list" class="btn btn-danger"><i class="fa fa-trash"></i>Xóa</a></li>
           </ul>'
        );
        $index++;
    }
  }
  return isset($rows) ? $rows : array();
}