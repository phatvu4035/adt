<?php
/*
* AJAX de load cac khu vuc dua tren cong ty
*/
function mac_load_region_by_company($form,$form_state) {
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
    $form['region_id']['#options'] = array(0=>t('Chọn')) + $matches;
    return $form['region_id'];
}

/*
* AJAX de load tinh thanh theo khu vuc
*/
function mac_load_province_by_region($form,$form_state) {
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

  $form['province_id']['#options'] = array(0=>t('Chọn')) + $matches;
  return $form['province_id'];
}


function mac_load_store_by_province($form,&$form_state) {
    $province_id = $form_state['values']['province_id'];
    $form['autocomplete_store_string_container']['store_string']['#autocomplete_path'] = 'mac/search_sap/'.$province_id;
    return $form['autocomplete_store_string_container'];
}


function load_mac_suggestion($mac_string = '') {
   $matches = array();
   $result = db_select('taxonomy_term_data','ttd')
        ->fields('ttd',array('tid','name'))
        ->condition('name','%'.db_like($mac_string).'%','LIKE')
        ->execute()
        ->fetchAll();
    foreach($result as $row) {
      $matches[$row->name] = $row->name;
    }

   drupal_json_output($matches);
}


function add_condition_search_mac($values = array(),$is_pager=1,$limit=0) {
    $voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $vid = $voca->vid;
    if($is_pager){
        $query = db_select('taxonomy_term_data','tdata')->extend('PagerDefault')->distinct()->limit($limit);
    }else{
        $query = db_select('taxonomy_term_data','tdata');
    }

    $query->leftJoin('field_data_field_mac_last_login','llogin','llogin.entity_id = tdata.tid');
    $query->condition('tdata.vid',$vid);

    if(isset($values['keyword']) && !empty($values['keyword'])) {
        $keyword = $values['keyword'];
        $keyword = trim($keyword);
        $query->condition('tdata.name','%'.db_like($keyword).'%','LIKE');
        $values['company'] = 0; 
        $values['region'] = 0;
        $values['province'] = 0;
        $values['store'] = 0; 
    }

    if(isset($values['macip']) && !empty($values['macip'])) {
        $macip = $values['macip'];
        $query->leftJoin('field_revision_field_mac_ip','macip','macip.entity_id = tdata.tid');
        $query->condition('macip.field_mac_ip_value','%'.db_like($macip).'%','LIKE');
        $values['company'] = 0;
        $values['region'] = 0;
        $values['province'] = 0;
        $values['store'] = 0;
    }

    if(isset($values['store_name']) && !empty($values['store_name'])) {
        $store_name = $values['store_name'];
        if(is_numeric($store_name)) {
            $query->leftJoin('field_data_field_mac_store_id','store','store.entity_id = tdata.tid');
            $query->leftJoin('field_data_field_store_sap','sap','sap.entity_id = store.field_mac_store_id_tid');
            $query->condition('sap.field_store_sap_value',$store_name);

        } else {
              $slice_store_name = explode(',', $store_name);
              if(count($slice_store_name) > 1) {
                  $query->leftJoin('field_data_field_mac_store_id','store','store.entity_id = tdata.tid');
                  $query->leftJoin('field_data_field_store_sap','sap','sap.entity_id = store.field_mac_store_id_tid');
                  $query->condition('sap.field_store_sap_value',$slice_store_name,'IN');
              } else {
                  $voca = taxonomy_vocabulary_machine_name_load('store');
                  $vid = $voca->vid;
                  $query->leftJoin('field_data_field_mac_store_id','store','store.entity_id = tdata.tid');
                  $store_query = db_select('taxonomy_term_data','tdata');
                  $store_query->condition('tdata.name','%'.db_like($store_name).'%','LIKE');
                  $store_query->condition('tdata.vid',$vid);
                  $store_query->fields('tdata',array('tid'));
                  $storeIds = $store_query->execute()->fetchCol();
                  if(count($storeIds) > 0) {
                      $query->condition('store.field_mac_store_id_tid',$storeIds,'IN');
                  } else {
                      return false;
                  }
              }
        }
        $values['company'] = 0;
        $values['region'] = 0;
        $values['province'] = 0;
    }

    if(isset($values['trangthai']) && (int)$values['trangthai'] > 0)  {
        $trangthai = $values['trangthai'];
        $curTime = time();
        $config = adstrue_mac_config();
        $codeTime = (int)$curTime - $config['status_time']*60;
        if($trangthai ==  1) {
            $query->condition('llogin.field_mac_last_login_value',$codeTime,'>');
        } else {
            $query->condition('llogin.field_mac_last_login_value',$codeTime,'<');
        }
    }
    
    if(isset($values['tinhtrang']) && (int)$values['tinhtrang'] > 0) {
        $tinhtrang = (int)$values['tinhtrang'];
        $query->leftJoin('field_data_field_device_status','dst','dst.entity_id = tdata.tid');
        $query->condition('dst.field_device_status_value',$tinhtrang);
    }

    if(isset($values['company']) && !empty($values['company'])){
        $company_id = $values['company'];
        $query->leftJoin('field_data_field_mac_company_id','maccid','maccid.entity_id = tdata.tid');
        $query->condition('maccid.field_mac_company_id_tid',$company_id);
    }
    if(isset($values['region']) && !empty($values['region'])){
        $region_id = $values['region'];
        $query->leftJoin('field_data_field_mac_region_id','macrgid','macrgid.entity_id = tdata.tid');
        $query->condition('macrgid.field_mac_region_id_tid',$region_id);
    }

    if(isset($values['province']) && !empty($values['province'])) {
        $province_id = $values['province'];
        $query->leftJoin('field_data_field_mac_province_id','macprid','macprid.entity_id = tdata.tid');
        $query->condition('macprid.field_mac_province_id_tid',$province_id);
    }
    if(isset($values['store']) && !empty($values['store'])) {
        $store_id = $values['store'];
        $query->leftJoin('field_data_field_mac_store_id','macs','macs.entity_id = tdata.tid');
        $query->condition('macs.field_mac_store_id_tid',$store_id);
    }
    if(isset($values['version_code']) && $values['version_code'] > 0) {
        $query->leftJoin('field_data_field_launcher_version','verc','verc.entity_id = tdata.tid');
        $query->condition('verc.  field_launcher_version_value',$values['version_code']);
    }
    if(isset($values['public_internet']) && (int)$values['public_internet'] > 0) {
        $public_internet  = (int)$values['public_internet'];
        if($public_internet == 1) {
            $query->leftJoin('field_data_field_public_internet','resource','resource.entity_id = tdata.tid');
            $db_or = db_or();
            $db_or->condition('resource.field_public_internet_value',NULL);
            $db_or->condition('resource.field_public_internet_value',1);
            $query->condition($db_or);
        } else {
            $query->leftJoin('field_data_field_public_internet','resource','resource.entity_id = tdata.tid');
            $query->condition('resource.field_public_internet_value',$public_internet);
        }
    }

    $query->fields('tdata',array('tid'));
    $query->orderBy('llogin.field_mac_last_login_value','DESC');
    $result = $query->execute();

    return $result;
}



function filter_region_by_company($tid) {
    $result = db_select('field_data_field_region_company_id','cpi')
              ->fields('cpi',array('entity_id'))
              ->condition('field_region_company_id_tid',$tid)
              ->execute()
              ->fetchCol();
    $regions = taxonomy_term_load_multiple($result);
    $rgSe = array(0 => t('Chọn vùng miền'));
    foreach($regions as $region) {
      $rgSe[$region->tid] =  $region->name;
    }
    return $rgSe;
}

function filter_province_by_region($tid) {
    $result = db_select('field_data_field_region_provice_id','rgi')
              ->fields('rgi',array('entity_id'))
              ->condition('field_region_provice_id_tid',$tid)
              ->execute()
              ->fetchCol();
    $provinces = taxonomy_term_load_multiple($result);
    $prSe = array(0 => t('Chọn tỉnh thành'));
    foreach($provinces as $province) {
      $prSe[$province->tid] =  $province->name;
    }
    return $prSe;
}

function filter_store_by_province($tid) {
   $result = db_select('field_data_field_province_store_id','pri')
              ->fields('pri',array('entity_id'))
              ->condition('field_province_store_id_tid',$tid)
              ->execute()
              ->fetchCol();
    $stores = taxonomy_term_load_multiple($result);
    $stSe = array(0 => t('Chọn tỉnh thành'));
    foreach($stores as $store) {
      $stSe[$store->tid] =  $store->name;
    }
    return $stSe;
}


function mac_load_region_by_company_table($form,$form_state) {
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
    $form['danhsach_mac']['header_filter']['region']['#options'] = array(0=>t('Chọn khu vực')) + $matches;
    $form['danhsach_mac']['header_filter']['region']['#prefix'] = '<div id="region-wrapper">';
    return $form['danhsach_mac']['header_filter']['region'];
}

function mac_load_province_by_region_table($form,$form_state) {
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

      $form['danhsach_mac']['header_filter']['province']['#options'] = array(0=>t('Chọn')) + $matches;
      $form['danhsach_mac']['header_filter']['province']['#prefix'] = '<div id="province-wrapper">';
      return $form['danhsach_mac']['header_filter']['province'];
}

function mac_load_store_by_province_table($form,&$form_state) {
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

    $form['danhsach_mac']['header_filter']['store']['#options'] = array(0=>t('Chọn')) + $matches;
    $form['danhsach_mac']['header_filter']['store']['#prefix'] = '<div id="store-wrapper" class="box-search-item">';
    return $form['danhsach_mac']['header_filter']['store'];
}

// Store Load suggestion MAc
function load_store_mac_suggestion($argument,$search_str) {
    $matches = array();
    $query = db_select('field_data_field_store_sap','stsap');

    $query->leftJoin('field_data_field_province_store_id','prid',"prid.entity_id = stsap.entity_id");
    $query->condition('prid.field_province_store_id_tid',$argument);

    $query->condition('field_store_sap_value','%'.db_like($search_str).'%','LIKE');
    $query->fields('stsap',array('entity_id'));
    $result  = $query->execute();

    if($result->rowCount()) {
        foreach($result as $row) {
           $store = taxonomy_term_load($row->entity_id);
           $sap = isset($store->field_store_sap['und'][0]['value']) ?  $store->field_store_sap['und'][0]['value'] : '';
           $store_id = $store->tid;
           $store_name = $store->name;
           $matches[$sap.'|_'.$store_name.'|_'.$store_id] = $sap.'_'.$store_name;
        }
    }

    drupal_json_output($matches);
}