<?php
function statistic_device_version_list_form($form,&$form_state){
    $form = array();
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $store_id = isset($_GET['store_id']) ? $_GET['store_id'] : 0;
    $version_code = isset($_GET['version_code']) ? $_GET['version_code'] : 0;
    $ma_sap = isset($_GET['sap']) ? $_GET['sap'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;
    $rows = array();
    if($ma_sap){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }

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
        '#markup' => '<h3>Thống kê Phiên bản thiết bị đang sử dụng</h3>',
        '#prefix' => '<div class="col-md-12">',
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
    $version_options = array(
        '0' => 'Chọn phiên bản',
        '1' => '1',
        '2' => '2'
    );
    $form['list_broadcast']['body']['broadcast_area']['version_code'] = array(
        '#type' => 'select',
        '#title' => 'Phiên bản',
        '#options' => $version_options,
        '#default_value' => $version_code,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
    );

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

    $form['list_broadcast']['body']['search_button'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>'

    );
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Cửa hàng'),
        array('data' => 'MAC'),
        array('data' => 'SAP'),
        array('data' => 'Phiên bản'),
    );
    //Load MAC
    $number_per_page = 50;
    $result = load_mac_data($number_per_page,$company_id,$region_id,$province_id,$store_id,$version_code,$ma_sap,$header);
    $total = $result['total'];
    $terms_result = $result['terms_result'];


    if($page){
        $count = ($page * $number_per_page)+1;
    }else{
        $count = 1;
    }

    if($total) {
        foreach ($terms_result as $term) {
            //Load store information

            $voca = taxonomy_vocabulary_machine_name_load('store');
            $terms_query = db_select('taxonomy_term_data','ttds');
            $terms_query->fields('ttds',array('tid','name'));
            $terms_query->fields('ss',array('field_store_sap_value'));
            $terms_query->leftJoin('field_data_field_store_sap','ss','ss.entity_id=ttds.tid');
            $terms_query->condition('tid',$term->field_mac_store_id_tid);
            $terms_result = $terms_query->execute()->fetchAssoc();

            $rows[] = array(
                $count,
                $terms_result['name'],
                $term->name,
                $terms_result['field_store_sap_value'],
                '<span class="badge bg-aqua">'.$term->field_launcher_version_value.'</span>',

            );
            $count++;
        }
    }
    if($version_code){
        $message = '<h4>Tổng số <b>'.$total.'</b> thiết bị đang sử dụng phiên bản '.$version_code.'</h4>';
    }else{
        $message = '<h4>Tổng số thiết bị : <b>'.$total.'</b></h4>';
    }

    $form['list_broadcast']['body']['search_summary'] = array(
        '#type' => 'item',
        '#markup' => '<div class="callout callout-success">'.$message.'</div>',
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
        '#markup' => theme('pager', array( 'element' => 0, 'parameters' => array( 'company_id' => $company_id,'region_id' => $region_id,'province_id' => $province_id,'store_id'=>$store_id,'sap' => $ma_sap))) ,
        '#weight' => 4,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    return $form;
}

function statistic_device_version_list_form_submit($form,&$form_state){
    global $base_url;
    $values = $form_state['values'];
    if(isset($values['ma_sap']) && $values['ma_sap']){
        $ma_sap = $values['ma_sap'];
    }else{
        $ma_sap = 0;
    }
    $company_id = isset($values['broadcast_company']) ? $values['broadcast_company'] : 0;
    $region_id = isset($values['broadcast_region']) ? $values['broadcast_region'] : 0;
    $province_id = isset($values['broadcast_province']) ? $values['broadcast_province'] : 0;
    $store_id = isset($values['broadcast_store']) ? $values['broadcast_store'] : 0;
    if($ma_sap){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
        'store_id' => $store_id,
        'version_code' => isset($values['version_code']) ? $values['version_code'] : 0,
        'sap' => $ma_sap,
        'page' => isset($values['page']) ? $values['page'] : 0,
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/statistic/device/version/list?'.$query_string;

}

function load_mac_data($number_per_page,$company_id,$region_id,$province_id,$store_id,$version_code,$ma_sap,$header){
    if($ma_sap){
        //Lấy store_id theo mã sap
        $voca_store = taxonomy_vocabulary_machine_name_load('store');
        $sap_query = db_select('taxonomy_term_data','ttds');
        $sap_query->fields('ttds',array('tid','name'));
        $sap_query->leftJoin('field_data_field_store_sap','ss','ss.entity_id=ttds.tid');
        $sap_query->condition('field_store_sap_value',trim($ma_sap));
        $sap_query->vid = $voca_store->vid;
        $sap_result = $sap_query->execute()->fetchAssoc();
        if($sap_result['tid']){
            $voca = taxonomy_vocabulary_machine_name_load('mac_address');
            $terms_query = db_select('taxonomy_term_data','ttd')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit($number_per_page);
            $terms_query->fields('ttd', array('tid','name'));
            $terms_query->fields('lv', array('field_launcher_version_value'));
            $terms_query->leftJoin('field_data_field_launcher_version','lv','lv.entity_id=ttd.tid');
            $terms_query->fields('mci', array('field_mac_store_id_tid'));
            $terms_query->leftJoin('field_data_field_mac_store_id','mci','mci.entity_id=ttd.tid');
            $terms_query->condition('ttd.vid', $voca->vid);

            $store_id = $sap_result['tid'];
            if ($store_id) {
                $terms_query->condition('field_mac_store_id_tid', $store_id);
            }
            if ($version_code) {
                $terms_query->condition('field_launcher_version_value', $version_code);
            }
            $num_rows = $terms_query->countQuery()->execute()->fetchField();
            $terms_result = $terms_query->execute();


        }else{
            $terms_result = array();
        }
        $result['terms_result'] = $terms_result;
        $result['total'] = isset($num_rows) ? $num_rows : 0;
        return $result;
    }else {
        $voca = taxonomy_vocabulary_machine_name_load('mac_address');
        $terms_query = db_select('taxonomy_term_data','ttd')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit($number_per_page);
        $terms_query->fields('ttd', array('tid','name'));
        $terms_query->fields('lv', array('field_launcher_version_value'));
        $terms_query->leftJoin('field_data_field_launcher_version','lv','lv.entity_id=ttd.tid');
        $terms_query->fields('mci', array('field_mac_store_id_tid'));
        $terms_query->leftJoin('field_data_field_mac_store_id','mci','mci.entity_id=ttd.tid');
        $terms_query->condition('ttd.vid', $voca->vid);
        if ($company_id) {
            $terms_query->leftJoin('field_data_field_mac_company_id', 'mci', 'mci.entity_id=ttd.tid');
            $terms_query->condition('field_mac_company_id_tid', $company_id);
        }
        if ($region_id) {
            $terms_query->leftJoin('field_data_field_mac_region_id', 'mri', 'mri.entity_id=ttd.tid');
            $terms_query->condition('field_mac_region_id_tid', $region_id);
        }
        if ($province_id) {
            $terms_query->leftJoin('field_data_field_mac_province_id', 'mpi', 'mpi.entity_id=ttd.tid');
            $terms_query->condition('field_mac_province_id_tid', $province_id);
        }
        if ($store_id) {
            $terms_query->condition('field_mac_store_id_tid', $store_id);
        }
        if ($version_code) {
            $terms_query->condition('field_launcher_version_value', $version_code);
        }
        $num_rows = $terms_query->countQuery()->execute()->fetchField();
        $terms_result = $terms_query->execute();
        $result['terms_result'] = $terms_result;
        $result['total'] = $num_rows;
        return $result;
    }
}

function statistic_device_monitoring_list_form($form,&$form_state){
    global $base_url;
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $status_id = isset($_GET['status']) ? $_GET['status'] : 0;
    $mac = isset($_GET['mac']) ? $_GET['mac'] : 0;

    $ma_sap = isset($_GET['sap']) ? $_GET['sap'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;
    $resource_id = isset($_GET['resource']) ? $_GET['resource'] : 0;

    $rows = array();
    if($ma_sap || $mac){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    //Lấy thông tin logging theo ngày
    $start_date = strtotime(date('Y-m-d 00:00:00'));
    $end_date = strtotime(date('Y-m-d 23:59:00'));
    $number_per_page = 50;

    $rows = array();

    $header = array(
        array('data' => 'STT'),
        array('data' => 'SAP'),
        array('data' => 'MAC'),
        array('data' => 'Đã tải'),
        array('data' => 'Số Files'),
        array('data' =>'Lịch phát sóng'),
        array('data' => 'Last Update'),
        array('data' =>'Nguồn phát'),
        array('data' => 'Trạng thái'),
        array('data'=>'Thao tác')
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
        '#markup' => '<h3>Thống kê tải file & chơi nội dung trên Thiết bị</h3>',
        '#prefix' => '<div class="col-md-12">',
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
    $status_options = array(
        '0' => 'Chọn trạng thái',
        '1' => 'Hoàn thành',
        '2' => 'Chưa hoàn thành'
    );
    $form['list_broadcast']['body']['broadcast_area']['status'] = array(
        '#type' => 'select',
        '#title' => 'Trạng thái',
        '#options' => $status_options,
        '#default_value' => $status_id,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
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
    /*
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
    );*/

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
        '#submit' => array('export_monitoring_statistic_submit')

    );
    $form['list_broadcast']['body']['broadcast_area']['search_button'] = array(
        '#type' => 'submit',
        '#weight' => 1,
        '#value' => 'Tìm kiếm',
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>',

    );

    //
    $result = load_logs($header,$number_per_page,$start_date,$end_date,$mac,$ma_sap,$page,$company_id,$region_id,$province_id,$resource_id,false,$status_id);
    $rows = $result['rows'];
    $total = $result['total'];

    $message = '<h4>Tổng số thiết bị là : '.$total.'</h4>';

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

// Export Excel
function export_monitoring_statistic_submit($form,&$form_state) {
    global $base_url;
    $path_class = drupal_get_path('module','adstrue_mac') .'/export_data_excel.php';
    include $path_class;

    $company_id = $form_state['values']['broadcast_company'];
    $region_id = $form_state['values']['broadcast_region'];
    $province_id = $form_state['values']['broadcast_province'];
    $status_id = $form_state['values']['status'];
    $mac = $form_state['values']['mac'];
    $resource_id = $form_state['values']['resource'];
    $ma_sap = $form_state['values']['ma_sap'];

    $rows = array();
    if($ma_sap || $mac){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }
    //Lấy thông tin logging theo ngày
    $start_date = strtotime(date('Y-m-d 00:00:00'));
    $end_date = strtotime(date('Y-m-d 23:59:00'));

    $exporter = new ExportDataExcel('browser', 'statistic_monitoring.xls');
    $exporter->initialize();
    $exporter->addRow(array('STT','SAP','MAC','Đã tải','Số Files','Lịch phát sóng','Last Update','Nguồn phát','Trạng thái','Thời gian Offline'));
    
    $logging_result = load_logs(array(),0,$start_date,$end_date,$mac,$ma_sap,0,$company_id,$region_id,$province_id,$resource_id,true,$status_id);

    if($logging_result->rowCount()) {
        $count = 1;
        foreach ($logging_result as $logging_id) {

            $logging = node_load($logging_id->nid);
            $mac = $logging->field_logging_mac['und'][0]['value'];

            $store_id = isset($logging->field_logging_store_id['und'][0]['tid']) ? $logging->field_logging_store_id['und'][0]['tid'] : null;
            $store = taxonomy_term_load($store_id);

            $broadcast_id = $logging->field_logging_broadcast_id['und'][0]['nid'];
            $broadcast = node_load($broadcast_id);

            // quyet dinh xem da hoan thanh chua
            $status_download = $logging->field_logging_status['und'][0]['value'];
            $num_file_download = $logging->field_num_file_downloaded['und'][0]['value'];

            if($status_download == 2) {
                $str_completed = 'Hoàn thành';
            } else {
                $str_completed = 'Chưa hoàn thành';
            }

            if(isset($store->field_store_sap['und'][0]['value'])){
                $device_link = $store->field_store_sap['und'][0]['value'];
            }else{
                $device_link = '';
            }

            $cur_time = time();
            $flag_time = (int)$logging_id->field_mac_last_login_value + 45 *60;
            $frendlyTime = '';

            if($flag_time > $cur_time) {
                $device_status = 'Online';

            } else {
                $device_status = 'Offline';
                $frendlyTime = secondsToFrendlyTime( (int)$flag_time );
            }

            $statsus_resource = '';
            if($logging_id->field_public_internet_value == 2) {
                $statsus_resource = 'Public';
            } else {
                $statsus_resource = 'Local';
            }
            $row = array(
                $count,
                $device_link,
                $logging->field_logging_mac['und'][0]['value'],
                $str_completed,
                $num_file_download,
                $broadcast->title,
                date('H:i:s d/m/Y',$logging->changed),
                $statsus_resource,
                $device_status,
                $frendlyTime
            );
            $exporter->addRow($row);
            $count++;
        }
    }
    $exporter->finalize();
    exit();

}

function statistic_device_monitoring_list_form_submit($form,&$form_state){
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
    $status_id = isset($values['status']) ? $values['status'] : 0;
    $resource_id = isset($values['resource']) ? $values['resource'] : 0;

    if($ma_sap || $mac){
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;
        $store_id = 0;
    }

    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
        'store_id' => $store_id,
        'mac' => $mac,
        'sap' => $ma_sap,
        'page' => isset($values['page']) ? $values['page'] : 0,
        'status' => $status_id,
        'resource' => $resource_id,
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/statistic/device/monitoring/list?'.$query_string;
}

function check_status_logging_download() {

    $start_date = strtotime(date('Y-m-d 00:00:00'));
    $end_date = strtotime(date('Y-m-d 23:59:00'));

    $query = db_select('node','n');
    $query->condition('n.type','logging_broadcast_playlist');
    $query->leftJoin('field_data_field_logging_mac','lm','lm.entity_id=n.nid');
    $query->condition('n.changed',$start_date,'>=');
    $query->condition('n.changed',$end_date,'<=');
    $query->leftJoin('field_data_field_logging_status','ls','ls.entity_id=n.nid');

    $query->leftJoin('field_data_field_logging_broadcast_id','lbid','lbid.entity_id = n.nid');
    // join voi bang broadcast id cua broadcast playlist time
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bid','bid.field_broadcast_playlist_time_id_nid = lbid.field_logging_broadcast_id_nid');
    // join voi bang play id cua content type broadcast playlist time 
    $query->leftJoin('field_data_field_playlist_broadcast_time_id','ptime','ptime.entity_id = bid.entity_id');
    $query->condition('ptime.field_playlist_broadcast_time_id_nid',array(8233,2829,128),'NOT IN');

    // Join company, region, province, store id
    $query->leftJoin('taxonomy_term_data','ttdata','ttdata.name = lm.field_logging_mac_value');
    $query->leftJoin('field_data_field_mac_store_id','macstore','macstore.entity_id = ttdata.tid');
    // Tu store lay company, region,province
    $query->leftJoin('field_data_field_company_store_id','cpstore','cpstore.entity_id = macstore.field_mac_store_id_tid');
    $query->leftJoin('field_data_field_region_store_id','rgstore','rgstore.entity_id = macstore.field_mac_store_id_tid');
    $query->leftJoin('field_data_field_province_store_id','prstore','prstore.entity_id = macstore.field_mac_store_id_tid');

    // lay ra cac truong store,province,region,company
    $query->fields('macstore',array('field_mac_store_id_tid'));
    $query->fields('cpstore',array('field_company_store_id_tid'));
    $query->fields('rgstore',array('field_region_store_id_tid'));
    $query->fields('prstore',array('field_province_store_id_tid'));

    $query->fields('n',array('nid'));
    $query->groupBy('n.nid');
    $query->orderBy('n.changed','DESC');
    $result = $query->execute();

    $voca_store = taxonomy_vocabulary_machine_name_load('store');
    $voca_mac = taxonomy_vocabulary_machine_name_load('mac_address');

    if($result->rowCount()) {
        //Load device information
        $completed = array();
        $uncompleted = array();

        foreach($result as $logging_id) {   ;
            $logging = node_load($logging_id->nid);
            $store_id = $logging_id->field_mac_store_id_tid;

            // Get company, region id , province id of store
            $company_id = isset($logging_id->field_company_store_id_tid) ? $logging_id->field_company_store_id_tid : null;
            $region_id = isset($logging_id->field_region_store_id_tid) ? $logging_id->field_region_store_id_tid : null;
            $province_id = isset($logging_id->field_province_store_id_tid) ? $logging_id->field_province_store_id_tid : null;


            // Lay cac file id da download duoc
            $downloaded_files = str_replace('[','',$logging->field_logging_downloaded['und'][0]['value']);
            $downloaded_files = str_replace(']','',$downloaded_files);
            // Tao thanh mang id cua cac file donload duoc
            $downloaded_files = explode(',',$downloaded_files);
            $downloaded_files = array_unique($downloaded_files);

            $number_downloaded_files = count($downloaded_files);

            // Lich phat song bao gom nhieu broadcast time
            $broadcast_id = $logging->field_logging_broadcast_id['und'][0]['nid'];

            //Calculating number files in a broadcast
            // lay slide trong logging playlist time thoi gian 
            $query = db_select('node','n');
            // id cua playlist
            $query->fields('pbt',array('field_playlist_broadcast_time_id_nid'));
            // Noi dung cua playlist
            $query->fields('sp',array('field_slides_playlist_nid'));
            $query->condition('n.type','broadcast_playlist_time');

            $query->leftJoin('field_data_field_broadcast_playlist_time_id','bpt','bpt.entity_id=n.nid');
            $query->condition('field_broadcast_playlist_time_id_nid',$broadcast_id);
            $query->leftJoin('field_data_field_playlist_broadcast_time_id','pbt','pbt.entity_id=n.nid');
            $query->leftJoin('node','pl','pl.nid=pbt.field_playlist_broadcast_time_id_nid');
            $query->condition('pl.type','playlist');
            $query->leftJoin('field_data_field_slides_playlist','sp','sp.entity_id=pl.nid');
            $query_result = $query->execute();

            $slide_nids  = array();
            // Lay toan bo cac file trong broad cast
            foreach($query_result as $slide){
                $slide_nids[] = $slide->field_slides_playlist_nid;
            }

            $slide_nids = array_unique($slide_nids);
            $compare = (array_intersect($slide_nids,$downloaded_files));

            // quyet dinh xem da hoan thanh chua
            if(count($slide_nids)==count($compare)){
                $logging->field_logging_status['und'][0]['value'] = 2;
                $logging->field_num_file_downloaded['und'][0]['value'] = count($compare).'/'.count($slide_nids);
                if(isset($company_id)) {
                    $logging->field_logging_company_id['und'][0]['tid'] = $company_id;
                }
                if(isset($region_id)) {
                    $logging->field_logging_region_id['und'][0]['tid'] = $region_id;
                }
                if(isset($province_id)) {
                    $logging->field_logging_province_id['und'][0]['tid'] = $province_id;
                }
                
                if(isset($store_id)) {
                    $logging->field_logging_store_id['und'][0]['tid'] = $store_id;
                }
                node_save($logging);

            }else {
                $logging->field_logging_status['und'][0]['value'] = 3;
                $logging->field_num_file_downloaded['und'][0]['value'] = count($compare).'/'.count($slide_nids);
                if(isset($company_id)) {
                    $logging->field_logging_company_id['und'][0]['tid'] = $company_id;
                }
                if(isset($region_id)) {
                    $logging->field_logging_region_id['und'][0]['tid'] = $region_id;
                }
                if(isset($province_id)) {
                    $logging->field_logging_province_id['und'][0]['tid'] = $province_id;
                }
                if(isset($store_id)) {
                    $logging->field_logging_store_id['und'][0]['tid'] = $store_id;
                }
                node_save($logging);
            }
        }
    }
    
}

function load_logs($header,$number_per_page,$start_date=0,$end_date=0,$mac=0,$sap=0,$page=0,$company_id=0,$region_id=0,$province_id=0,$resource_id = 0,$is_exporting = false,$status = 0) {
    global $base_url;
    global $pager_total_items;
    global $pager_total;
    $count = ($number_per_page*$page)+1;
    // check_status_logging_download($start_date,$end_date);

    $voca = taxonomy_vocabulary_machine_name_load('mac_address');
    $rows = array();
    if(!$is_exporting) {
        $query = db_select('node','n')->extend('PagerDefault')->extend('TableSort')->orderByHeader($header)->distinct()->limit($number_per_page);
    } else {
        $query = db_select('node','n');
    }

    $query->fields('n',array('nid'));
    //$query->groupBy('field_logging_mac_value');
    $query->condition('n.type','logging_broadcast_playlist');
    $query->leftJoin('field_data_field_logging_mac','lm','lm.entity_id = n.nid');

    $query->leftJoin('taxonomy_term_data','ttd','ttd.name = lm.field_logging_mac_value');
    $query->leftJoin('field_data_field_mac_last_login','llg','llg.entity_id = ttd.tid');

    $query->leftJoin('field_data_field_logging_broadcast_id','lbid','lbid.entity_id = n.nid');
    // join voi bang broadcast id cua broadcast playlist time
    $query->leftJoin('field_data_field_broadcast_playlist_time_id','bid','bid.field_broadcast_playlist_time_id_nid = lbid.field_logging_broadcast_id_nid');
    // join voi bang play id cua content type broadcast playlist time 
    $query->leftJoin('field_data_field_playlist_broadcast_time_id','ptime','ptime.entity_id = bid.entity_id');
    $query->condition('ptime.field_playlist_broadcast_time_id_nid',array(8233,2829,128),'NOT IN');
    $query->fields('llg',array('field_mac_last_login_value'));


    $query->leftJoin('field_data_field_public_internet','res','res.entity_id = ttd.tid');
    if($resource_id) {
        if($resource_id == 1) {
            $db_or = db_or();
            $db_or->condition('res.field_public_internet_value',NULL);
            $db_or->condition('res.field_public_internet_value',1);
            $query->condition($db_or);
        } else {
            $query->condition('res.field_public_internet_value',$resource_id);
        } 
    }
    $query->fields('res',array('field_public_internet_value'));
    $query->condition('n.changed',$start_date,'>=');
    $query->condition('n.changed',$end_date,'<=');
    $query->orderBy('n.changed','DESC');

    // Mac dinh gia tri cua mac la rong neu co mac thi se gan vao bien nay
    if($mac) {
        $query->condition('lm.field_logging_mac_value',trim($mac));
    }

    $mac = '';
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
            $terms_query = db_select('taxonomy_term_data', 'ttd');
            $terms_query->fields('ttd', array('tid', 'name'));
            $terms_query->condition('ttd.vid', $voca->vid);
            $terms_query->leftJoin('field_data_field_mac_store_id','msi','msi.entity_id=ttd.tid');
            $terms_query->condition('field_mac_store_id_tid',$sap_result['tid']);
            $terms_result = $terms_query->execute()->fetchAssoc();
            $mac = $terms_result['name'];
            if($mac) {
                $query->condition('field_logging_mac_value',$mac);
            } else {
                $query->condition('field_logging_mac_value',false);
            }
        }
    }

    // Load by company/region/province
    if($company_id){
        $query->leftJoin('field_data_field_logging_company_id','cpi','cpi.entity_id=n.nid');
        $query->condition('cpi.field_logging_company_id_tid',$company_id);
    }
    if($region_id){
        $query->leftJoin('field_data_field_logging_region_id','rgi','rgi.entity_id=n.nid');
        $query->condition('rgi.field_logging_region_id_tid',$region_id);
    }
    if($province_id){
        $query->leftJoin('field_data_field_logging_province_id','pri','pri.entity_id=n.nid');
        $query->condition('pri.field_logging_province_id_tid',$province_id);
    }
    if($status == 1) {
        $query->leftJoin('field_data_field_logging_status','ls','ls.entity_id=n.nid');
        $query->condition('ls.field_logging_status_value',2);
    }
    if($status == 2) {
        $array_status = array(1,3);
        $query->leftJoin('field_data_field_logging_status','ls','ls.entity_id=n.nid');
        $query->condition('ls.field_logging_status_value',$array_status,'IN');
    }

    if($mac){
        $query->condition('lm.field_logging_mac_value',trim($mac));
    }

    $query->orderBy('n.created','DESC');

    $logging_result = $query->execute();

    // Neu muon xuat ra excel tra ve ket qua luon
    if($is_exporting) {
        return $logging_result;
    }
    $result = array();
    $result['total'] = (int)$pager_total_items[0];

    // kiem tra ca truong hop co mac hay khong khi loc theo sap, cong ty, cung mien
    if($logging_result->rowCount() ){

        foreach($logging_result as $logging_id){

            $logging = node_load($logging_id->nid);
            $mac = $logging->field_logging_mac['und'][0]['value'];

            $store_id = isset($logging->field_logging_store_id['und'][0]['tid']) ? $logging->field_logging_store_id['und'][0]['tid'] : null;
            $store = taxonomy_term_load($store_id);

            $broadcast_id = $logging->field_logging_broadcast_id['und'][0]['nid'];
            $broadcast = node_load($broadcast_id);

            // quyet dinh xem da hoan thanh chua
            $status_download = $logging->field_logging_status['und'][0]['value'];
            $num_file_download = (isset($logging->field_num_file_downloaded['und'][0]['value'])) ? $logging->field_num_file_downloaded['und'][0]['value'] : '';
            if($status_download == 2) {
                $str_completed = '<span class="label label-success">Hoàn thành</span>';
            } else {
                $str_completed = '<span class="label label-warning">Chưa hoàn thành</span>';
            }

            if(isset($store->field_store_sap['und'][0]['value'])){
                $device_link = '<a href="'.$base_url.'/mac/list?get=1&store_name='.$store->field_store_sap['und'][0]['value'].'">'.$store->field_store_sap['und'][0]['value'].'</a>';
            }else{
                $device_link = '';
            }
            $cur_time = time();
            $flag_time = (int)$logging_id->field_mac_last_login_value + 45 *60;
            if($flag_time > $cur_time) {
                $device_status = '<span class="label label-success">Online</span>';
            } else {
                $offTime = $cur_time - $flag_time;
                $friendlyTime = secondsToFrendlyTime((int)$flag_time);
                $device_status = '<span class="label label-danger">Offline - '.$friendlyTime.'</span></p>';
            }
            $statsus_resource = '';
            if($logging_id->field_public_internet_value == 2) {
                $statsus_resource = 'Public';
            } else {
                $statsus_resource = 'Local';
            }
            $rows[] = array(
                $count,
                $device_link,
                $logging->field_logging_mac['und'][0]['value'],
                $str_completed,
                $num_file_download,
                isset($broadcast->title) ? '<a href="'.$base_url.'/broadcast/view/playlist/'.$broadcast->nid.'">'.$broadcast->title.'</a>' : '',
                date('H:i:s d/m/Y',$logging->changed),
                $statsus_resource,
                $device_status,
                '<a href="'.$base_url.'/statistic/device/monitoring/view/'.$logging->nid.'" class="btn btn-success"><i class="fa fa-eye"></i> Chi tiết</a>'
            );
            $count++;
        }
    }
    // Cap nhat lai so trang voi cac trang thai chua hoan thanh hoac hoan thanh
    $result['rows'] = $rows;
    return $result;
}

function statistic_device_monitoring_view_form($form,&$form_state,$logging_id){
    global $base_url;
    $count = 1;
    if($logging_id){
        $logging = node_load($logging_id);
        $downloaded_files = str_replace('[','',$logging->field_logging_downloaded['und'][0]['value']);
        $downloaded_files = str_replace(']','',$downloaded_files);
        $downloaded_files = explode(',',$downloaded_files);
        $downloaded_files = array_unique($downloaded_files);
        $number_downloaded_files = count($downloaded_files);

        $broadcast = node_load($logging->field_logging_broadcast_id['und'][0]['nid']);
        $rows = array();
        if($downloaded_files){
            foreach($downloaded_files as $slide_nid){
                $slide = node_load($slide_nid);
                $term = taxonomy_term_load($slide->field_slide_category_content['und'][0]['value']);
                $media = '';
                $media_path = isset($slide->field_link_open_file['und'][0]['value']) ? $slide->field_link_open_file['und'][0]['value'] : file_create_url($slide->field_slide_file['und'][0]['uri']);

                switch ($slide->field_media_type['und'][0]['value']){
                    case 1:
                        $media = '<video width="250" controls="controls">
                                        <source src="'.$media_path.'" type="video/mp4" autostart="false">
                                  </video>';
                        break;
                    case 2:
                        break;
                    case 3:
                        $media = '<img src="'.$media_path.'" width="250"/>';
                        break;

                }
                //Load user create the slide
                $username = user_load($slide->uid);

                $rows[] = array(
                    $count,
                    $slide->title,
                    $media,
                    $term->name,
                    gmdate('H:i:s',$slide->field_slide_duration['und'][0]['value']),
                    $username->name,
                    date('d/m/Y',$slide->created)

                );
                $count++;
            }
        }

    }
    $form['loggin_info'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
    );
    $form['loggin_info']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['header']['playlist_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Thông tin chung</h3>',
        '#prefix' => '<div class="col-md-10">',
        '#suffix' => '</div>'
    );

    $back_url = $base_url.'/statistic/device/monitoring/list';

    $form['loggin_info']['header']['back_playlist'] = array(
        '#type' => 'item',
        '#markup' => '<a href="'.$back_url.'" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>',
    );
    $form['loggin_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['body']['is_playling_broadcast'] = array(
        '#type' => 'item',
        '#markup' => '<b>Lịch phát sóng : </b>' . $broadcast->title,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['body']['start_date'] = array(
        '#type' => 'item',
        '#markup' => '<b>Từ ngày : </b>' . date('d/m/Y',$broadcast->field_start_time_playlist['und'][0]['value']),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['body']['end_date'] = array(
        '#type' => 'item',
        '#markup' => '<b>Đến ngày : </b>' . date('d/m/Y',$broadcast->field_end_time_playlist['und'][0]['value']),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['body']['total_file_downloaded'] = array(
        '#type' => 'item',
        '#markup' => '<b>Tổng số file đã download : </b>' . $number_downloaded_files,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $header = array(
        array('data' => 'STT'),
        array('data' => 'Tên nội dung'),
        array('data' => 'Nội dung'),
        array('data' => 'Thời lượng'),
        array('data' => 'Nhóm Nội dung'),
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
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
        '#markup' => '<h3>Chi tiết file đã download trên Thiết bị</h3>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
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
    return $form;
}


function statistic_app_version_list_form($form,&$form_state){
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->condition('n.type','apps');
    $query->leftJoin('field_data_field_app_type','at','at.entity_id=n.nid');
    $query->condition('field_app_type_value',1);

    $result = $query->execute()->fetchAssoc();
    $launcher = node_load($result['nid']);
    $header = array(
        array('data' => 'Version Code'),
        array('data' => 'Version Name'),
        array('data' => 'Dung lượng'),
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
        array('data' => 'File'),
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
        '#markup' => '<h3>Thông tin phiên bản đang sử dụng trên Thiết bị</h3>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $user = user_load($launcher->uid);

    $link_download = isset($launcher->field_file_apk_cdn['und'][0]['value']) ? $launcher->field_file_apk_cdn['und'][0]['value'] : file_create_url($launcher->field_filename['und'][0]['uri']);
    $rows[] = array(
        $launcher->field_version_code['und'][0]['value'],
        $launcher->field_version_name['und'][0]['value'],
        isset($launcher->field_app_file_size['und'][0]['value']) ? $launcher->field_app_file_size['und'][0]['value'] : 0,
        $user->name,
        date('d/m/Y H:i:s',$launcher->created),
        '<a href="'.$link_download.'" class="btn btn-info"><i class="fa fa-cloud-download"></i> Tải về</a>',
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
    return $form;
}


function statistic_cron_cleanup() {
    db_delete('semaphore')
        ->condition('name','cron')
        ->execute();
}


?>