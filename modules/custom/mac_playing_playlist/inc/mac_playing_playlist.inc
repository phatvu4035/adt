<?php

function mac_playlist_playing($form,&$form_state) {
  global $base_url;
	$form = array();
	$header = array(
	    array('data' => 'STT'),
	    array('data' => 'Địa chỉ MAC'),
	    array('data' => 'Cửa hàng'),
	    array('data' => 'SAP'),
	    array('data' => 'Broadcast'),
	    array('data' => 'Playlist đang chơi'),
      array('data' => 'Nội dung đã tải'),
	    array('data' => 'Thời gian chơi'),
	);

	// Lay toan bo list cong ty sau do cac truong khu vuc, tinh thanh, chon dan theo
    $cVoc = taxonomy_vocabulary_machine_name_load('company');
    $cVid = $cVoc->vid;
    $companies = $terms = taxonomy_term_load_multiple(array(), array('vid' => $cVid));
    $cSelect =  array(0=>t('Chọn'));
    foreach($companies as $company) {
      $cSelect[$company->tid] = $company->name;
    }

    $values = isset($form_state['values']) ? $form_state['values'] : array();
    if(isset($_GET['get']) && $_GET['get'] == 1) {
        $values = $_GET;
    }

    $result = build_form_mac_playing($values);

	  if(isset($values['company']) && (int)$values['company'] > 0) {
        $company_id = $values['company'];
        $rgSe = filter_region_by_company($company_id);
    }

    if(isset($values['region']) && (int)$values['region'] > 0) {
        $company_id = $values['company'];
        $region_id = $values['region'];
        $rgSe = filter_region_by_company($company_id);

        $prSe  = filter_province_by_region($region_id);
    }
    if(isset($values['province']) && (int)$values['province'] > 0) {
        $company_id = $values['company'];
        $region_id = $values['region'];
        $province_id = $values['province'];
        $rgSe = filter_region_by_company($company_id);
        $prSe  = filter_province_by_region($region_id);
        $stSe = filter_store_by_province($prSe);
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

	$rows = array();
	if($result->rowCount()) {
		$index = 1;
		foreach($result as $row) {
			$logging_broadcast_playlist = node_load($row->entity_id);
			$logging_downloaded = isset($logging_broadcast_playlist->field_logging_downloaded['und'][0]['value']) ? $logging_broadcast_playlist->field_logging_downloaded['und'][0]['value'] : '';
      $logging_downloaded = explode(',', $logging_downloaded);

      $downloaded_content = '';
      foreach($logging_downloaded as $downloaed) {
          $downloaded_content .= '<a href="node/'.$downloaed.'">'.$downloaed.' , </a>';
      }

			$mac = taxonomy_term_load($row->tid);
			$region_id = isset($mac->field_mac_region_id['und'][0]['tid']) ? $mac->field_mac_region_id['und'][0]['tid'] : '';
			$region = taxonomy_term_load($region_id);
			$province_id = isset($mac->field_mac_province_id['und'][0]['tid']) ? $mac->field_mac_province_id['und'][0]['tid'] : '';
			$province = taxonomy_term_load($province_id);
			$store_id = isset($mac->field_mac_store_id['und'][0]['tid']) ? $mac->field_mac_store_id['und'][0]['tid'] : '';
			$store = taxonomy_term_load($store_id);
			$mac_ip = isset($mac->field_mac_ip['und'][0]['value']) ? $mac->field_mac_ip['und'][0]['value'] : '';
      $sap_display = '';
			$sap = isset($store->field_store_sap['und'][0]['value']) ? $store->field_store_sap['und'][0]['value'] : '';
      if($sap) {
        $sap_display = '<a href="'.$base_url.'/mac/list/?get=1&mac_sap='.$sap.'">'.$sap.'</a>';
      }

			$logging_playlist = isset($logging_broadcast_playlist->field_logging_playlist_id['und'][0]['value']) ? $logging_broadcast_playlist->field_logging_playlist_id['und'][0]['value'] : '';
      
			$logging_playlist_arr = json_decode($logging_playlist,true);
      if(count($logging_playlist_arr)  > 0) {
          usort($logging_playlist_arr,'cmp');
      }

			if(count($logging_playlist_arr) > 0) {
				foreach($logging_playlist_arr as $logging) {
					$playlist_id = $logging['playlist_id'];
					$playlist = node_load($playlist_id);
					$broadcast_id = $logging['broadcast_id'];
					$broadcast = node_load($broadcast_id);
					$time_playling = $logging['time_is_playing'];
					$time_playing_format = date('H:i:s d/m/Y',$time_playling);
					$rows[] = array(
						$index,
						(isset($mac->name)) ? $mac->name : '',
						(isset($store->name)) ? $store->name : '',
						$sap_display,
						(isset($broadcast->title)) ? $broadcast->title : '',
						(isset($playlist->title)) ? $playlist->title : '',
            isset($downloaded_content) ? $downloaded_content : '',
						(isset($time_playing_format)) ? $time_playing_format : '',
					);
					$index++;
				}
			}
		}
		$index++;
	}

	//body of table
	$form['mac_playlist_playing'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success','wrapper-form')
        ),
    );

    $form['mac_playlist_playing']['header_filter'] = array(
    	'#type' => 'container',
        '#prefix' => '<div class="box-header with-border" id="header-filter">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('box','box-header')
        ),
    );

    $form['mac_playlist_playing']['header_filter']['company'] = array(
         '#type' => 'select',
         '#title' => t('Công ty'),
         '#options' => $cSelect,
         '#ajax' => array(
            'callback' => 'mac_playing_region_by_company_table',
            'method' =>'replace',
            'wrapper' => 'region-wrapper',
          ),
         '#default_value' => isset($company_id) ? $company_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item">',
         '#suffix' => '</div>',
    );
    $form['mac_playlist_playing']['header_filter']['region'] = array(
         '#type' => 'select',
         '#title' => t('Vùng miền'),
         '#options' => (isset($rgSe)) ? $rgSe : array(0 => t('Chọn vùng miền')),
         '#ajax' => array(
            'callback' => 'mac_playing_province_by_region_table',
            'method' =>  'replace',
            'wrapper' => 'province-wrapper',
          ),
         '#default_value' => isset($region_id) ? $region_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="region-wrapper">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE,
    );
    $form['mac_playlist_playing']['header_filter']['province'] = array(
         '#type' => 'select',
         '#title' => t('Tỉnh thành'),
         '#options' => isset($prSe) ? $prSe : array(0 => t('Chọn tỉnh thành')),
         '#ajax' => array(
            'callback' => 'mac_playing_store_by_province_table',
            'method' =>  'replace',
            'wrapper' => 'store-wrapper',
          ),
         '#default_value' => isset($province_id) ? $province_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="province-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE
    );
    $form['mac_playlist_playing']['header_filter']['store'] = array(
         '#type' => 'select',
         '#title' => t('Cửa hàng'),
         '#options' => isset($stSe) ? $stSe : array(0 => t('Chọn cửa hàng')),
         '#default_value' => isset($store_id) ? $store_id : 0,
         '#prefix' => '<div class="col-md-3 box-search-item"><div id="store-wrapper" class="box-search-item">',
         '#suffix' => '</div></div>',
         '#validated' => TRUE
    );
    $form['mac_playlist_playing']['header_filter']['keyword'] = array(
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
    $form['mac_playlist_playing']['header_filter']['macip'] = array(
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

    $form['mac_playlist_playing']['header_filter']['mac_sap'] = array(
        '#type' => 'textfield',
        '#size' => 60,
        '#title' => t('Mã SAP'),
        '#attributes' => array(
          'placeholder' => 'Tìm MAC SAP',
        ),
        '#default_value' => isset($mac_sap) ? $mac_sap : NULL,
        '#maxlength' => 60,
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
    );
    $form['mac_playlist_playing']['header_filter']['submit_search'] = array(
        '#type' => 'submit',
        '#weight' => 2,
        '#value' => t('Tìm kiếm'),
        '#prefix' => '<div class="col-md-2 box-search-item">',
        '#suffix' => '</div>',
        '#submit' => array('mac_playlist_playing_search_submit'),
    );

    $form['mac_playlist_playing']['body'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>',

    );
    $form['mac_playlist_playing']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );
    return $form;
}

/* Sorting an array */
function cmp($a,$b) {
  return $b['time_is_playing'] - $a['time_is_playing'];
}

function build_form_mac_playing($values) {
	$current_time = time();
  $limit = 2;
	$voca_mac = taxonomy_vocabulary_machine_name_load('mac_address');
	$mac_vid = $voca_mac->vid;
	$query = db_select('taxonomy_term_data','tdata')->extend('PagerDefault')->distinct()->limit($limit);
	$query->condition('tdata.vid',$mac_vid);
	$query->innerJoin('field_data_field_logging_mac','loggmac','loggmac.field_logging_mac_value = tdata.name');

  if(isset($values['keyword']) && !empty($values['keyword'])) {
     $keyword = $values['keyword'];
     $keyword = trim($keyword);
     $query->condition('tdata.name','%'.db_like($keyword).'%','LIKE');
     $values['company'] = $values['region'] = $values['province'] = 0;
  }
  
  if(isset($values['macip']) && !empty($values['macip'])) {
      $macip = $values['macip'];
      $query->leftJoin('field_revision_field_mac_ip','macip','macip.entity_id = tdata.tid');
      $query->condition('macip.field_mac_ip_value','%'.db_like($macip).'%','LIKE');
      $values['company'] = $values['region'] = $values['province'] = 0;
  }
  if(isset($values['mac_sap']) && !empty($values['mac_sap'])) {
      $masap = $values['mac_sap'];
      $idStore = db_select('field_data_field_store_sap','ssap')
              ->fields('ssap',array('entity_id'))
              ->condition('field_store_sap_value',$masap)
              ->execute()
              ->fetchCol();
      if(count($idStore) > 0) {
          $storeIds = array(0);
          $query->leftJoin('field_data_field_mac_store_id','macst','macst.entity_id = tdata.tid');
          $query->condition('macst.field_mac_store_id_tid',$idStore,"IN"); 
      }
      $values['company'] = $values['region'] = $values['province'] = 0;
  }

	if(isset($values['company']) && (int)$values['company'] > 0) {
  		$company_id  = $values['company'];
  		$query->leftJoin('field_data_field_mac_company_id','cpi','cpi.entity_id = tdata.tid');
  		$query->condition('cpi.field_mac_company_id_tid',$company_id);
	}
	if(isset($values['region']) && (int)$values['region'] > 0) {
  		$region_id = $values['region'];
  		$query->leftJoin('field_data_field_mac_region_id','rgi','rgi.entity_id = tdata.tid');
  		$query->condition('rgi.field_mac_region_id_tid',$region_id);
	}

	if(isset($values['province']) && (int)$values['province'] > 0) {
  		$province_id = $values['province'];
  		$query->leftJoin('field_data_field_mac_province_id','pri','pri.entity_id = tdata.tid');
  		$query->condition('pri.field_mac_province_id_tid',$province_id);
	}

	$query->fields('loggmac',array('entity_id'));
	$query->fields('tdata',array('tid'));
	$result = $query->execute();
	return $result;

}

function mac_playlist_playing_search_submit($form,&$form_state) {
	global $base_url;
    $values  = $form_state['values'];
    $get = '?get=1';
    foreach($values as $key => $val) {
        if($key !== 'submit_search' && $key !== 'form_build_id' && $key !== 'form_token' && $key !== 'form_id' && $key !== 'op') {
            $get .= '&'.$key.'='.$val;
        }
    }
    $form_state['redirect'] = $base_url.'/mac-playing-playlist'.$get;
}

function mac_playing_region_by_company_table($form,$form_state) {
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
    $form['mac_playlist_playing']['header_filter']['region']['#options'] = array(0=>t('Chọn khu vực')) + $matches;
    $form['mac_playlist_playing']['header_filter']['region']['#prefix'] = '<div id="region-wrapper">';
    return $form['mac_playlist_playing']['header_filter']['region'];
}

function mac_playing_province_by_region_table($form,$form_state) {
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

      $form['mac_playlist_playing']['header_filter']['province']['#options'] = array(0=>t('Chọn')) + $matches;
      $form['mac_playlist_playing']['header_filter']['province']['#prefix'] = '<div id="province-wrapper">';
      return $form['mac_playlist_playing']['header_filter']['province'];
}

function mac_playing_store_by_province_table($form,&$form_state) {
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

    $form['mac_playlist_playing']['header_filter']['store']['#options'] = array(0=>t('Chọn')) + $matches;
    $form['mac_playlist_playing']['header_filter']['store']['#prefix'] = '<div id="store-wrapper" class="box-search-item">';
    return $form['mac_playlist_playing']['header_filter']['store'];

}