<?php
function content_dashboard(){
    global $base_url;
    //Tổng số files
    $query = db_select('node','n');
    $query->fields('n',array('nid','type'));
    $db_or = db_or();
    $db_or->condition('n.type','slides');
    $db_or->condition('n.type','playlist');
    $db_or->condition('n.type','broadcast_playlist');
    $query->condition($db_or);
    $result = $query->execute();
    $number_files = 0;
    $number_playlist = 0;
    $number_broadcast_waitting = 0;
    $number_broadcast = 0;
    if($result->rowCount()){
        foreach($result as $row){
            switch ($row->type){
                case 'slides':
                    $number_files++;
                    break;
                case 'playlist':
                    $number_playlist++;
                    break;
                case 'broadcast_playlist':
                    $number_broadcast++;
                    break;

            }
        }
    }
    //Get the list of broadcast not approved
    $broadcast_query = db_select('node','n');
    $broadcast_query->fields('n',array('nid'));
    $broadcast_query->condition('n.type','broadcast_playlist');
    $broadcast_query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
    $db_or = db_or();
    $db_or->condition('field_approval_playlist_value',1);
    $db_or->condition('field_approval_playlist_value',3);
    $broadcast_query->condition($db_or);
    $broadcast_result = $broadcast_query->execute();

    $number_broadcast_waitting = ($broadcast_result->rowCount()) ? $broadcast_result->rowCount() : 0;
    $str_files = '<div class="small-box bg-aqua">
                    <div class="inner">
                        <h3>'.$number_files.'</h3>
                        <p>Nội dung</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-file-video-o"></i>
                  </div>
                    <a href="'.$base_url.'/noidung/add" class="small-box-footer"><i class="fa fa-plus"></i> Tạo nội dung</a>
                  </div>';
    $str_playlists = '<div class="small-box bg-green">
                    <div class="inner">
                        <h3>'.$number_playlist.'</h3>
                        <p>Playlist</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-list"></i>
                  </div>
                    <a href="'.$base_url.'/manage_playlist/create" class="small-box-footer"><i class="fa fa-plus"></i> Tạo Playlist</a>
                  </div>';
    $str_broadcast = '<div class="small-box bg-yellow">
                    <div class="inner">
                        <h3>'.$number_broadcast.'</h3>
                        <p>Lịch phát sóng</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-calendar"></i>
                  </div>
                    <a href="'.$base_url.'/broadcast/create" class="small-box-footer"><i class="fa fa-plus"></i> Tạo lịch phát sóng</a>
                  </div>';
    $str_broadcast_waiting = '<div class="small-box bg-red">
                    <div class="inner">
                        <h3>'.$number_broadcast_waitting.'</h3>
                        <p>Lịch phát sóng chờ duyệt</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-calendar-check-o"></i>
                  </div>
                    <a href="'.$base_url.'/broadcast/review/list" class="small-box-footer"><i class="fa fa-check"></i> Duyệt lịch phát sóng</a>
                  </div>';
    $form['summary_block'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['files'] = array(
        '#type' => 'item',
        '#markup' => $str_files,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['playlist'] = array(
        '#type' => 'item',
        '#markup' => $str_playlists,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['broadcast'] = array(
        '#type' => 'item',
        '#markup' => $str_broadcast,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['broadcast_waiting'] = array(
        '#type' => 'item',
        '#markup' => $str_broadcast_waiting,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    return $form;
}

function device_dashboard(){
    global $base_url;
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
        $variables  = array();
        $online = 0;
        $offline = 0;
        $dakhoa = 0;
        $dakichhoat = 0;
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

    $str_online_device = '<div class="small-box bg-green">
                    <div class="inner">
                        <h3>'.$online.'</h3>
                        <p>Thiết bị Online</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-tv"></i>
                  </div>
                    <a href="'.$base_url.'/mac/list?get=1&trangthai=1" class="small-box-footer">Xem chi tiết  <i class="fa fa-arrow-circle-right"></i></a>
                  </div>';
    $str_offline_device = '<div class="small-box bg-red">
                    <div class="inner">
                        <h3>'.$offline.'</h3>
                        <p>Thiết bị Offline</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-warning"></i>
                  </div>
                    <a href="'.$base_url.'/mac/list?get=1&trangthai=2" class="small-box-footer">Xem chi tiết  <i class="fa fa-arrow-circle-right"></i></a>
                  </div>';
    $str_active_device = '<div class="small-box bg-aqua">
                    <div class="inner">
                        <h3>'.$dakichhoat.'</h3>
                        <p>Thiết bị đã kích hoạt</p>
                    </div>
                  <div class="icon">
                    <i class="fa  fa-check-circle-o"></i>
                  </div>
                    <a href="'.$base_url.'/mac/list?get=1&tinhtrang=1" class="small-box-footer">Xem chi tiết  <i class="fa fa-arrow-circle-right"></i></a>
                  </div>';
    $str_total_device = '<div class="small-box bg-yellow">
                    <div class="inner">
                        <h3>'.$tongso.'</h3>
                        <p>Tổng số thiết bị</p>
                    </div>
                  <div class="icon">
                    <i class="fa fa-database"></i>
                  </div>
                    <a href="'.$base_url.'/mac/list" class="small-box-footer">Xem chi tiết  <i class="fa fa-arrow-circle-right"></i></a>
                  </div>';

    $form['summary_block'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="row">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['online_device'] = array(
        '#type' => 'item',
        '#markup' => $str_online_device,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['offline_device'] = array(
        '#type' => 'item',
        '#markup' => $str_offline_device,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['active_device'] = array(
        '#type' => 'item',
        '#markup' => $str_active_device,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    $form['summary_block']['total_device'] = array(
        '#type' => 'item',
        '#markup' => $str_total_device,
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>'
    );
    return $form;
}

function broadcast_dashboard(){
    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Từ ngày/Tới ngày'),
        array('data'=>'Lịch phát sóng'),
        array('data' => 'Ngày còn lại'),
        array('data' => 'Trang thái'),
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
        array('data' => 'Công ty'),
        array('data' => 'Vùng miền'),
        array('data' => 'Tỉnh/Thành'),
        array('data' => 'Cửa hàng'),
        array('data'=>'Thao tác'),

    );
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $store_id = isset($_GET['store_id']) ? $_GET['store_id'] : 0;
    $status = isset($_GET['status']) ? $_GET['status'] : 0;
    $start_date = isset($_GET['start_date']) ? $_GET['start_date'] : 0;
    $end_date = isset($_GET['end_date']) ? $_GET['end_date'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;

    $number_per_page = 10;
    $start_date = date('Y-m-01');
    //$end_date = (date('Y-m-t'));


    $rows = list_broadcast($company_id,$region_id,$province_id,$store_id,$status,$start_date,$end_date,$header,$number_per_page,$page);

    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box')
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
        '#markup' => '<h3>Lịch phát sóng tháng '.date('m/Y').'</h3>',

    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );

    $form['list_broadcast']['body']['broadcasts'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows['rows'],
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );

    return $form;
}

function timeline_dashboard(){
    //drupal_add_css(drupal_get_path('module','adtrue_block').'/assets/css/timetablejs.css');
    drupal_add_js(drupal_get_path('module','adtrue_block').'/assets/js/jq.schedule.js');
    //drupal_add_js(drupal_get_path('module','adtrue_block').'/assets/js/timetable.min.js');
    drupal_add_css(drupal_get_path('module','adtrue_block').'/assets/css/style.css');
    //drupal_add_js(drupal_get_path('module','adtrue_block').'/assets/js/timeline.js',array('scope' => 'footer'));


    /*
     * Lấy lịch phát sóng hiện tại
     */
    //Vinmart+ CMSMKT
    $device_company_id = 49;
    $device_region_id = 0;
    $device_province_id = 0;
    $device_store_id = 0;
    $broadcast_current_id = 0;

    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->fields('stp',array('field_start_time_playlist_value'));
    $query->condition('n.type','broadcast_playlist');
    //check theo công ty
    $query->leftJoin('field_data_field_company_id_playlist','cip','cip.entity_id=n.nid');
    $query->condition('field_company_id_playlist_tid',$device_company_id);
    //check theo vùng mi?n
    $query->leftJoin('field_data_field_region_id_playlist','rip','rip.entity_id=n.nid');
    $query->condition('field_region_id_playlist_tid',$device_region_id);
    //check theo t?nh thành
    $query->leftJoin('field_data_field_province_id_playlist','pip','pip.entity_id=n.nid');
    $query->condition('field_province_id_playlist_tid',$device_province_id);

    $query->leftJoin('field_data_field_level_access','la','la.entity_id=n.nid');
    $query->condition('field_level_access_value',4);
    //check theo c?a hàng
    //$query->leftJoin('field_data_field_store_id_playlist','sip','sip.entity_id=n.nid');
    //$query->condition('field_store_id_playlist_tid',$device_store_id);
    //Check di?u ki?n l?ch phát sóng
    $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
    $query->condition('field_approval_playlist_value',2);
    //L?y l?ch phát sóng ngày hi?n t?i
    $current_date = strtotime(date('Y-m-d'));

    $query->leftJoin('field_data_field_start_time_playlist','stp','stp.entity_id=n.nid');
    $query->condition('field_start_time_playlist_value',$current_date,'<=');
    $query->leftJoin('field_data_field_end_time_playlist','etp','etp.entity_id=n.nid');
    $query->condition('field_end_time_playlist_value',$current_date,'>=');

    $query->orderBy('stp.field_start_time_playlist_value','DESC');
    $query->orderBy('n.created','DESC');

    $broadcasts = $query->execute();

    if($broadcasts->rowCount()){
        foreach($broadcasts as $broadcast_nid){
            $broadcast_id = $broadcast_nid->nid;

            $broadcast = node_load($broadcast_id);
            $stores = $broadcast->field_store_id_playlist['und'];                                                                                                                 foreach($stores as $store_id){
                if($store_id['tid']==$device_store_id){
                    $broadcast_current_id = $broadcast_id;
                    break;
                }
            }
            if($broadcast_current_id){
                break;
            }
        }
    }


    if(!$broadcast_current_id){

        $query = db_select('node', 'n');
        $query->fields('n', array('nid'));
        $query->condition('n.type', 'broadcast_playlist');
        //check theo công ty
        $query->leftJoin('field_data_field_company_id_playlist', 'cip', 'cip.entity_id=n.nid');
        $query->condition('field_company_id_playlist_tid', $device_company_id);
        //check theo vùng mi?n
        $query->leftJoin('field_data_field_region_id_playlist', 'rip', 'rip.entity_id=n.nid');
        $query->condition('field_region_id_playlist_tid', $device_region_id);
        //check theo t?nh thành
        $query->leftJoin('field_data_field_province_id_playlist', 'pip', 'pip.entity_id=n.nid');
        $query->condition('field_province_id_playlist_tid', $device_province_id);

        $query->leftJoin('field_data_field_level_access','la','la.entity_id=n.nid');
        $query->condition('field_level_access_value',3);

        //Check di?u ki?n l?ch phát sóng
        $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
        $query->condition('field_approval_playlist_value',2);
        //L?y l?ch phát sóng ngày hi?n t?i
        $query->leftJoin('field_data_field_start_time_playlist','stp','stp.entity_id=n.nid');
        $query->condition('field_start_time_playlist_value',$current_date,'<=');
        $query->leftJoin('field_data_field_end_time_playlist','etp','etp.entity_id=n.nid');
        $query->condition('field_end_time_playlist_value',$current_date,'>=');

        $query->orderBy('n.created', 'DESC');
        $query->range(0, 1);
        $broadcast_result = $query->execute()->fetchAssoc();

        if(isset($broadcast_result['nid']) && $broadcast_result['nid']) {
            $broadcast_current_id = $broadcast_result['nid'];

        }else{

            $query = db_select('node', 'n');
            $query->fields('n', array('nid'));
            $query->condition('n.type', 'broadcast_playlist');
            //check theo công ty
            $query->leftJoin('field_data_field_company_id_playlist', 'cip', 'cip.entity_id=n.nid');
            $query->condition('field_company_id_playlist_tid', $device_company_id);
            //check theo vùng mi?n
            $query->leftJoin('field_data_field_region_id_playlist', 'rip', 'rip.entity_id=n.nid');
            $query->condition('field_region_id_playlist_tid', $device_region_id);

            $query->leftJoin('field_data_field_level_access','la','la.entity_id=n.nid');
            $query->condition('field_level_access_value',2);

            //Check di?u ki?n l?ch phát sóng
            $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
            $query->condition('field_approval_playlist_value',2);

            //L?y l?ch phát sóng ngày hi?n t?i
            $query->leftJoin('field_data_field_start_time_playlist','stp','stp.entity_id=n.nid');
            $query->condition('field_start_time_playlist_value',$current_date,'<=');
            $query->leftJoin('field_data_field_end_time_playlist','etp','etp.entity_id=n.nid');
            $query->condition('field_end_time_playlist_value',$current_date,'>=');
            $query->orderBy('n.created', 'DESC');
            $query->range(0, 1);
            $broadcast_result = $query->execute()->fetchAssoc();
            if(isset($broadcast_result['nid']) && $broadcast_result['nid']) {
                $broadcast_current_id = $broadcast_result['nid'];
            }else{

                $query = db_select('node', 'n');
                $query->fields('n', array('nid'));
                $query->condition('n.type', 'broadcast_playlist');
                //check theo công ty
                $query->leftJoin('field_data_field_company_id_playlist', 'cip', 'cip.entity_id=n.nid');
                $query->condition('field_company_id_playlist_tid', $device_company_id);

                $query->leftJoin('field_data_field_level_access','la','la.entity_id=n.nid');
                $query->condition('field_level_access_value',1);

                //Check di?u ki?n l?ch phát sóng
                $query->leftJoin('field_data_field_approval_playlist','ap','ap.entity_id=n.nid');
                $query->condition('field_approval_playlist_value',2);
                //L?y l?ch phát sóng ngày hi?n t?i
                $query->leftJoin('field_data_field_start_time_playlist','stp','stp.entity_id=n.nid');
                $query->condition('field_start_time_playlist_value',$current_date,'<=');
                $query->leftJoin('field_data_field_end_time_playlist','etp','etp.entity_id=n.nid');
                $query->condition('field_end_time_playlist_value',$current_date,'>=');
                $query->orderBy('n.created', 'DESC');
                $query->range(0, 1);
                $broadcast_result = $query->execute()->fetchAssoc();
                if(isset($broadcast_result['nid']) && $broadcast_result['nid']) {
                    $broadcast_current_id = $broadcast_result['nid'];

                }
            }
        }
    }
    if(!$broadcast_current_id){
        $broadcast_current_id = 901;
    }
    $broadcast = node_load($broadcast_current_id);

    $playlists = load_broadcast_info($broadcast_current_id);
    $addLocations = 'timetable.addLocations([';

    $addEvent = '';
    $count = 0;

    foreach($playlists as $playlist){

        if($count < (count($playlists)-1)){
            $addLocations .='\''.$playlist['title'].'\',';
        }else{
            $addLocations .='\''.$playlist['title'].'\'';
        }


        $addEvent .= 'timetable.addEvent(\''.$playlist['title'].'\', \''.$playlist['title'].'\', new Date('.date('Y').','.date('m').','.date('d').','.date('H',$playlist['start']).','.date('i',$playlist['start']).'), new Date('.date('Y').','.date('m').','.date('d').','.date('H',$playlist['end']).','.date('i',$playlist['end']).'));';
        $count++;

    }
    $addLocations .=']);';

    $start_time = date('H',$playlists[0]['start']);
    $end_time = date('H',($playlists[count($playlists)-1]['end'] + 60*60));

/*
    $js = '<script>(function ($) {
    Drupal.behaviors.adtrue = {
        attach: function (context, settings) {

            var timetable = new Timetable();

            timetable.setScope('.$start_time.','.$end_time.');
            '.$addLocations.$addEvent.'
            
            var renderer = new Timetable.Renderer(timetable);
            renderer.draw(\'.timetable\');
        }
    };
    })(jQuery);
    </script>';
*/
    $js = '<script>(function ($) {
    Drupal.behaviors.adtrue = {        
        attach: function (context, settings) {
            
            var $sc = $("#schedule").timeSchedule({
        startTime: "07:00", // schedule start time(HH:ii)
        endTime: "22:00",   // schedule end time(HH:ii)
        widthTime:60 * 10,  // cell timestamp example 10 minutes
        timeLineY:60,       // height(px)
        verticalScrollbar:20,   // scrollbar (px)
        timeLineBorder:1,   // border(top and bottom)
        debug:"#debug",     // debug string output elements
        rows : {
            \'1\' : {
                title : \'Title Area\',
                schedule:[
                    {
                        start:\'09:00\',
                        end:\'12:00\',
                        text:\'Text Area\',
                        data:{
                        }
                    },
                    {
                        start:\'11:00\',
                        end:\'14:00\',
                        text:\'Text Area\',
                        data:{
                        }
                    }
                ]
            },
            \'2\' : {
                title : \'Title Area\',
                schedule:[
                    {
                        start:\'16:00\',
                        end:\'17:00\',
                        text:\'Text Area\',
                        data:{
                        }
                    }
                ]
            }
        },
        change: function(node,data){
            alert("change event");
        },
        init_data: function(node,data){
        },
        click: function(node,data){
            alert("click event");
        },
        append: function(node,data){
        },
        time_click: function(time,data){
            alert("time click event");
        },
    });
        }
    };
    })(jQuery);
    </script>';
    $form['list_broadcast'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box box-info')
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
        '#markup' => '<h3>Đang phát : '.$broadcast->title.'</h3>',

    );
    $form['list_broadcast']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['timeline'] = array(
        '#type' => 'item',
        '#markup' => '<div class="timetable" id="schedule"></div>',
        '#weight' => 2,
    );

    $form['list_broadcast']['body']['js'] = array(
        '#type' => 'item',
        '#markup' => $js,
        '#weight' => 2
    );

    return $form;
}


function load_broadcast_info($broadcast_current_id){
    $query = db_select('node', 'n');
    $query->fields('n',array('nid'));
    $query->fields('pl',array('title'));
    $query->fields('pbt',array('field_playlist_broadcast_time_id_nid'));
    $query->fields('bps',array('field_broadcast_playlist_start_value'));
    $query->fields('bpe',array('field_broadcast_playlist_end_value'));
    $query->condition('n.type', 'broadcast_playlist_time');
    $query->leftJoin('field_data_field_broadcast_playlist_time_id', 'bpt', 'bpt.entity_id=n.nid');
    $query->condition('field_broadcast_playlist_time_id_nid', $broadcast_current_id);
    $query->leftJoin('field_data_field_broadcast_playlist_start','bps','bps.entity_id=n.nid');
    $query->leftJoin('field_data_field_broadcast_playlist_end','bpe','bpe.entity_id=n.nid');

    $query->leftJoin('field_data_field_playlist_broadcast_time_id','pbt','pbt.entity_id=n.nid');
    $query->leftJoin('node','pl','pl.nid=pbt.field_playlist_broadcast_time_id_nid');
    $query->orderBy('field_broadcast_playlist_start_value','ASC');

    $playlist_result = $query->execute();
    $i = 0;
    foreach($playlist_result as $row){
        $playlist = node_load($row->field_playlist_broadcast_time_id_nid);
        $rows[$i]['playlist_id'] = $row->field_playlist_broadcast_time_id_nid;
        $rows[$i]['title'] = $row->title;
        $rows[$i]['start'] = $row->field_broadcast_playlist_start_value;
        $rows[$i]['end'] = $row->field_broadcast_playlist_end_value;
        $i++;
    }
    return $rows;
}



?>

