<?php

function statistic_device_fail_list_form($form,&$form_state) {
	$company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $status_id = isset($_GET['status']) ? $_GET['status'] : 0;
    $mac = isset($_GET['mac']) ? $_GET['mac'] : 0;

    $ma_sap = isset($_GET['sap']) ? $_GET['sap'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;
    $resource_id = isset($_GET['resource']) ? $_GET['resource'] : 0;
    $active_id = isset($_GET['active']) ? $_GET['active'] : 0;

    $rows = array();
    if($ma_sap || $mac){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
        $resource_id = 0;
    }

    $start_date = strtotime(date('Y-m-d 00:00:00'));
    $end_date = strtotime(date('Y-m-d 23:59:00'));
    $number_per_page = 50;


	$header = array(
        array('data' => 'STT'),
        array('data' => 'SAP'),
        array('data' => 'MAC'),
        array('data' => 'Công ty'),
        array('data' => 'Vùng miền'),
        array('data' => 'Tỉnh thành'),
        array('data' => 'Last Update'),
        array('data' => 'Trạng thái'),
        array('data' => 'Nguồn phát'),
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
    $form['list_broadcast']['header']['header_title'] = array(
        '#type' => 'item',
        '#weight' => 2,
        '#markup' => '<h3>Thống kê các thiết bị không tải được file</h3>',
        '#prefix' => '<div class="col-md-12 ">',
        '#suffix' => '</div>'

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
    $resource = array(
        0 => 'Chọn nguồn phát',
        1 => 'Local',
        2 => 'Public'
    );

    $form['list_broadcast']['body']['broadcast_area']['resource'] = array(
        '#type' => 'select',
        '#title' => 'Chọn nguồn phát',
        '#options' => $resource,
        '#default_value' => $resource_id,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );
    $active = array(
        0 => 'Tình trạng',
        1 => 'Online',
        2 => 'Offline'
    );
    $form['list_broadcast']['body']['broadcast_area']['active'] = array(
        '#type' => 'select',
        '#title' => 'Tình trạng',
        '#options' => $active,
        '#default_value' => $active_id,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );
    if($mac){
        $form['list_broadcast']['body']['broadcast_area']['mac'] = array(
            '#type' => 'textfield',
            '#title' => 'MAC',
            '#default_value' => $mac,
            '#prefix' => '<div class="col-md-3">',
            '#suffix' => '</div>',
        );
    }else{
        $form['list_broadcast']['body']['broadcast_area']['mac'] = array(
            '#type' => 'textfield',
            '#title' => 'MAC',
            '#prefix' => '<div class="col-md-3">',
            '#suffix' => '</div>',
        );
    }

    if($ma_sap){
        $form['list_broadcast']['body']['broadcast_area']['ma_sap'] = array(
            '#type' => 'textfield',
            '#title' => 'SAP',
            '#default_value' => $ma_sap,
            '#prefix' => '<div class="col-md-3">',
            '#suffix' => '</div>',
        );
    }else{
        $form['list_broadcast']['body']['broadcast_area']['ma_sap'] = array(
            '#type' => 'textfield',
            '#title' => 'SAP',
            '#prefix' => '<div class="col-md-3">',
            '#suffix' => '</div>',
        );
    }

    $form['list_broadcast']['body']['broadcast_area']['export-button'] = array(
        '#type' => 'submit',
        '#value' => 'Export',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-3 pull-right" style="margin-top:25px;">',
        '#suffix' => '</div>',
        '#attributes' => array('class'=>array('btn','btn-warning')),
        '#submit' => array('export_monitoring_statistic_fail_submit')

    );
    $form['list_broadcast']['body']['broadcast_area']['search_button'] = array(
        '#type' => 'submit',
        '#weight' => 1,
        '#value' => 'Tìm kiếm',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',

    );

    $result = load_fails(50,$header,$start_date,$end_date,$mac,$ma_sap,$page,$company_id,$region_id,$province_id,$resource_id,false,$active_id);

    $rows = $result['rows'];
    $total = $result['total'];
    $c = $result['c'];
    $message = '<h4 class="'.$c.'">Tổng số thiết bị là : '.$total.'</h4>';

    $form['list_broadcast']['body']['search_summary'] = array(
        '#type' => 'item',
        '#markup' => '<div class="callout callout-info">'.$message.'</div>',
        '#prefix' => '<div class="col-md-12" style="margin-top : 30px;">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['result'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('class'=>array('table table-striped')),
        '#empty' => t('Không có thiết bị nào.'),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['body']['pager'] = array(
        '#type' => 'item',
        '#markup' => theme('pager'),//, array( 'element' => 0, 'parameters' => array( 'company_id' => $company_id,'region_id' => $region_id,'province_id' => $province_id,'store_id'=>$store_id,'sap' => $ma_sap))) ,
        '#weight' => 4,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    return $form;
}

function load_fails($number_per_page,$header,$start_date,$end_date,$mac=0,$sap=0,$page=0,$company_id=0,$region_id=0,$province_id=0,$resource_id,$is_exporting = false,$active = 0) {
	global $base_url;
    global $pager_total_items;
    $count = ($number_per_page*$page)+1;
	// Lay tat ca cac mac dang log
	$query_log = db_select('field_data_field_logging_mac','lm');
	$query_log->leftJoin('node','n','n.nid = lm.entity_id');
	$query_log->condition('n.changed',$start_date,'>=');
    $query_log->condition('n.changed',$end_date,'<=');
    $query_log->leftJoin('taxonomy_term_data','ttd','ttd.name = lm.field_logging_mac_value');

    $query_log->fields('ttd',array('tid'));
    $tid_logging = $query_log->execute()->fetchCol();
    $tid_logging = array_unique($tid_logging);

    $c = count($tid_logging);

    $voca = taxonomy_vocabulary_machine_name_load('mac_address');

    $mac_tax = taxonomy_term_load_multiple(array(),array('vid' => $voca->vid));
    $mac_ids = array_keys($mac_tax);
    $undownloaded_mac_id = array_diff($mac_ids,$tid_logging);


    // Lay  mac chua duoc logging
    if(!$is_exporting) {
        $query = db_select('taxonomy_term_data','ttdata')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit($number_per_page);
    } else {
        $query = db_select('taxonomy_term_data','ttdata');
    }

    $query->condition('ttdata.vid',$voca->vid);
    
    if(count($undownloaded_mac_id) < 1) {
        $query->condition('ttdata.tid',0, '=');
    } else {
        $query->condition('ttdata.tid',$undownloaded_mac_id, 'IN');
    }
    // $query->condition('ttdata.tid',$tid_logging, 'NOT IN');
    
    if($sap){
        //Load store id
        $voca_store = taxonomy_vocabulary_machine_name_load('store');
        $sap_query = db_select('taxonomy_term_data','ttds');
        $sap_query->fields('ttds',array('tid','name'));
        $sap_query->leftJoin('field_data_field_store_sap','ss','ss.entity_id=ttds.tid');
        $sap_query->condition('field_store_sap_value',trim($sap));
        $sap_query->vid = $voca_store->vid;
        $sap_result = $sap_query->execute()->fetchAssoc();
        // lay sap =>store =>tid =>mac
        if($sap_result['tid']) {
            $terms_query = db_select('taxonomy_term_data', 'taxtd');
            $terms_query->fields('taxtd', array('tid', 'name'));
            $terms_query->condition('taxtd.vid', $voca->vid);
            $terms_query->leftJoin('field_data_field_mac_store_id','msi','msi.entity_id=taxtd.tid');
            $terms_query->condition('field_mac_store_id_tid',$sap_result['tid']);
            $terms_result = $terms_query->execute()->fetchAssoc();
            $mac = $terms_result['name'];
        }
    }

    if($mac) {
        $query->condition('ttdata.name',$mac,'=');
    } else if(!$mac && $sap) {
        $query->condition('ttdata.name',false,'=');
    }

    $query->leftJoin('field_data_field_public_internet','resource','resource.entity_id = ttdata.tid');

    if($resource_id) {
    	if($resource_id == 1) {
    		$db_or = db_or();
            $db_or->condition('resource.field_public_internet_value',NULL);
            $db_or->condition('resource.field_public_internet_value',1);
            $query->condition($db_or);
    	} else {
    		$query->condition('resource.field_public_internet_value',$resource_id);
    	}
    }
    // Join store cua mac voi entity = tid cua mac
    $query->leftJoin('field_data_field_mac_store_id','store','store.entity_id = ttdata.tid');
    // Lay SAP cua store 
    $query->leftJoin('field_data_field_store_sap','sap','sap.entity_id = store.field_mac_store_id_tid');
    // Join tat cac cac bang cua cong ty, tinh thanh, ... cua store
    $query->leftJoin('field_data_field_company_store_id','cpi','cpi.entity_id = store.field_mac_store_id_tid');

    $query->leftJoin('field_data_field_region_store_id','rgi','rgi.entity_id = store.field_mac_store_id_tid');
    $query->leftJoin('field_data_field_province_store_id','pri','pri.entity_id = store.field_mac_store_id_tid');

    // Lay lan cap nhat lan cuoi cua mac
    $query->leftJoin('field_data_field_mac_last_login','lastlogin','lastlogin.entity_id = ttdata.tid');
    // Tim kiem online offline
    if($active) {
        $curtime = time();
        $flagtime = time()- 45*60;
        if($active == 1) {
            $query->condition('lastlogin.field_mac_last_login_value',$flagtime,'>=');
        } else {
            $query->condition('lastlogin.field_mac_last_login_value',$flagtime,'<=');
        }
    }

    if($company_id){
        $query->condition('cpi.field_company_store_id_tid',$company_id);
    }

    if($region_id){
        $query->condition('rgi.field_region_store_id_tid',$region_id);
    }

    if($province_id){
        $query->condition('pri.field_province_store_id_tid',$province_id);
    }

    $query->fields('ttdata',array('tid','name'));
    $query->fields('sap',array('field_store_sap_value'));
	$query->fields('cpi',array('field_company_store_id_tid'));
	$query->fields('rgi',array('field_region_store_id_tid'));
	$query->fields('pri',array('field_province_store_id_tid'));
	$query->fields('lastlogin',array('field_mac_last_login_value'));
	$query->fields('resource',array('field_public_internet_value'));

    $result = $query->execute();
    // Neu muon xuat ra excel tra ve ket qua luon
    if($is_exporting) {
        return $result;
    }
    $output['total'] = $pager_total_items[0];
    $rows = array();

    if($result->rowCount()) {
    	foreach($result as $row) {
    		$mac = $row->name;
    		if(isset($row->field_store_sap_value) ){
                $device_link = '<a href="'.$base_url.'/mac/list?get=1&store_name='.$row->field_store_sap_value.'">'.$row->field_store_sap_value.'</a>';
            }else{
                $device_link = '';
            }

            $company = isset($row->field_company_store_id_tid) ? taxonomy_term_load($row->field_company_store_id_tid) : '';
            $region = isset($row->field_region_store_id_tid) ? taxonomy_term_load($row->field_region_store_id_tid) : '';
            $province = isset($row->field_province_store_id_tid) ? taxonomy_term_load($row->field_province_store_id_tid) : '';

            $last_login = isset($row->field_mac_last_login_value) ? date('H:i:s d-m-Y',$row->field_mac_last_login_value) : '';

            if( isset($row->field_mac_last_login_value) ) {
            	$last_update = $row->field_mac_last_login_value;
            	$flag_time = (int)$last_update + 45*60;
            	$cur_time = time();
            	if($flag_time > $cur_time) {
            		$device_status = '<span class="label label-success">Online</span>';
            	} else {
            		$offTime = $cur_time - $flag_time;
	                $friendlyTime = secondsToFrendlyTime((int)$flag_time);
	                $device_status = '<span class="label label-danger">Offline - '.$friendlyTime.'</span></p>';
            	}
            }
            $resource = '';
            if(isset($row->field_public_internet_value) && $row->field_public_internet_value == 2) {
            	$resource = 'Pulic';
            } else {
            	$resource = 'Local';
            }

            $rows[] = array(
            	$count,
            	$device_link,
            	$row->name,
            	($company) ? $company->name : '' ,
            	($region) ? $region->name : '',
            	($province) ? $province->name : '',
            	$last_login,
            	$device_status,
            	$resource
            );

            $count++;
    	}
    }
    $output['rows'] = $rows;
    $output['c'] = $c;
    return $output;
}


// Submit form
function statistic_device_fail_list_form_submit($form,&$form_state) {
	global $base_url;
    $values = $form_state['values'];
    if(isset($values['ma_sap']) && $values['ma_sap']){
        $ma_sap = $values['ma_sap'];
    }else{
        $ma_sap = 0;
    }
    if(isset($values['mac']) && $values['mac']){
        $mac= $values['mac'];
    }else{
        $mac = 0;
    }
    $company_id = isset($values['broadcast_company']) ? $values['broadcast_company'] : 0;
    $region_id = isset($values['broadcast_region']) ? $values['broadcast_region'] : 0;
    $province_id = isset($values['broadcast_province']) ? $values['broadcast_province'] : 0;
    $store_id = isset($values['broadcast_store']) ? $values['broadcast_store'] : 0;
    $resource_id = isset($values['resource']) ? $values['resource'] : 0;
    $active_id = isset($values['active']) ? $values['active'] : 0;

    if($ma_sap || $mac){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
        $active_id = 0;
    }

    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
        'store_id' => $store_id,
        'mac' => $mac,
        'sap' => $ma_sap,
        'page' => isset($values['page']) ? $values['page'] : 0,
        'resource' => $resource_id,
        'active' => $active_id
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/statistic/device/fail/list?'.$query_string;
}


// Export Excel
function export_monitoring_statistic_fail_submit($form,&$form_state) {
    global $base_url;
    $path_class = drupal_get_path('module','adstrue_mac') .'/export_data_excel.php';
    include $path_class;

    $company_id = $form_state['values']['broadcast_company'];
    $region_id = $form_state['values']['broadcast_region'];
    $province_id = $form_state['values']['broadcast_province'];
    $mac = $form_state['values']['mac'];
    $resource_id = $form_state['values']['resource'];
    $ma_sap = $form_state['values']['ma_sap'];
    $active = $form_state['values']['active'];

    $rows = array();
    if($ma_sap || $mac){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
        $resource_id = 0;
    }
    //Lấy thông tin logging theo ngày
    $start_date = strtotime(date('Y-m-d 00:00:00'));
    $end_date = strtotime(date('Y-m-d 23:59:00'));

    
    $exporter = new ExportDataExcel('browser', 'statistic_monitoring_fail.xls');
    $exporter->initialize();
    $exporter->addRow(array('STT','SAP','MAC','Công ty','Vùng miền','Tỉnh thành','Last Update','Nguồn phát','Trạng thái','Thời gian offline'));

    
    $rows = load_fails(50,array(),$start_date,$end_date,$mac,$ma_sap,0,$company_id,$region_id,$province_id,$resource_id,true,$active);
    if($rows->rowCount()) {
        foreach ($rows as $row) {
        	$mac = $row->name;

    		if(isset($row->field_store_sap_value) ){
                $device_link = $row->field_store_sap_value;
            }else{
                $device_link = '';
            }

            $company = isset($row->field_company_store_id_tid) ? taxonomy_term_load($row->field_company_store_id_tid) : '';
            $region = isset($row->field_region_store_id_tid) ? taxonomy_term_load($row->field_region_store_id_tid) : '';
            $province = isset($row->field_province_store_id_tid) ? taxonomy_term_load($row->field_province_store_id_tid) : '';

            $last_login = isset($row->field_mac_last_login_value) ? date('H:i:s d-m-Y',$row->field_mac_last_login_value) : '';

            if( isset($row->field_mac_last_login_value) ) {
            	$last_update = $row->field_mac_last_login_value;
            	$flag_time = (int)$last_update + 45*60;
            	$cur_time = time();
            	$friendlyTime = '';
            	if($flag_time > $cur_time) {
            		$device_status = 'Online';
            	} else {
            		$offTime = $cur_time - $flag_time;
	                $friendlyTime = secondsToFrendlyTime((int)$flag_time);
	                $device_status = 'Offline';
            	}
            }
            $resource = '';
            if(isset($row->field_public_internet_value) && $row->field_public_internet_value == 2) {
            	$resource = 'Pulic';
            } else {
            	$resource = 'Local';
            }


            $record = array(
            	$count,
            	$device_link,
            	$row->name,
            	($company) ? $company->name : '' ,
            	($region) ? $region->name : '',
            	($province) ? $province->name : '',
            	$last_login,
            	$device_status,
            	$resource,
            	$friendlyTime,
            );

            $exporter->addRow($record);
            $count++;
        }
    }
    $exporter->finalize();
    exit();

}