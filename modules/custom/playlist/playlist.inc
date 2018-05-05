<?php

function _get_update_launcher($mac,$mac_eth,$version_code){
    $data = array();
    if($mac || $mac_eth) {
        $query = db_select('taxonomy_term_data', 'ttd');
        $query->fields('ttd', array('tid'));
        $query->fields('lv',array('field_launcher_version_value'));
        $query->leftJoin('field_data_field_launcher_version','lv','lv.entity_id=ttd.tid');
        $db_or = db_or();
        $db_or->condition('ttd.name', $mac);
        $db_or->condition('ttd.name', $mac_eth);
        $query->condition($db_or);
        $result_term = $query->execute()->fetchAssoc();
        if ($result_term['tid']) {
            $query = db_select('node', 'n');
            $query->fields('n', array('nid'));
            $query->leftJoin('field_data_field_app_type', 'fat', 'fat.entity_id=n.nid');
            $query->condition('n.type', 'apps');
            $query->condition('fat.field_app_type_value', 1);
            $query->condition('n.status', 1);
            $query->range(0, 1);
            $result = $query->execute()->fetchAssoc();
            $data = array();
            if ($result) {
                $app = node_load($result['nid']);
                $data['filename'] = file_create_url(($app->field_filename['und']['0']['uri']));
                $data['version_code'] = $app->field_version_code['und']['0']['value'];
                $data['version_name'] = $app->field_version_name['und']['0']['value'];
                $data['update_type'] = $app->field_app_update_type['und'][0]['value'];
                //$data['filesize'] = $app->field_app_file_size['und'][0]['value'];
                $data['created'] = $app->created;
                $data['updated'] = $app->changed;
                $data['result'] = true;
                //Update version code for MAC
                if($result_term['tid']!=$version_code) {
                    $update_term = taxonomy_term_load($result_term['tid']);
                    $update_term->field_launcher_version['und'][0]['value'] = $version_code;
                    $update_term->field_last_update_version['und'][0]['value'] = strtotime(date('Y-m-d H:i:s'));
                    taxonomy_term_save($update_term);
                }
            } else {
                $data['result'] = false;
            }


        }else{
            $query = db_select('node', 'n');
            $query->fields('n', array('nid'));
            $query->leftJoin('field_data_field_app_type', 'fat', 'fat.entity_id=n.nid');
            $query->condition('n.type', 'apps');
            $query->condition('fat.field_app_type_value', 1);
            $query->condition('n.status', 1);
            $query->range(0, 1);
            $result = $query->execute()->fetchAssoc();
            $data = array();
            if ($result) {
                $app = node_load($result['nid']);
                $data['filename'] = file_create_url(($app->field_filename['und']['0']['uri']));
//                $data['filename'] = 'http://cmsmkt.vingroup.net/Vin_app-att-1_Test_Update_Apk.apk';
                $data['version_code'] = $app->field_version_code['und']['0']['value'];
                $data['version_name'] = $app->field_version_name['und']['0']['value'];
                $data['update_type'] = $app->field_app_update_type['und'][0]['value'];
                //$data['filesize'] = $app->field_app_file_size['und'][0]['value'];
                $data['created'] = $app->created;
                $data['updated'] = $app->changed;
                $data['result'] = true;
            } else {
                $data['result'] = false;
            }
        }

    }else{
        $data['result'] = false;
    }

    return $data;
}

function _logging_update_launcher_service_create($mac,$version_code,$version_name,$update_type){
    global $user;
    if($mac && $version_code && $version_name && $update_type) {
        $logging = new stdClass();
        $logging->type = 'launcher_update_logs';
        $logging->title = $mac;
        $logging->status = 1;
        $logging->language = 'en';
        $logging->uid = 1;
        $logging->field_launcher_version_code['und'][0]['value'] = $version_code;
        $logging->field_launcher_version_name['und'][0]['value'] = $version_name;
        $logging->field_launcher_update_type['und'][0]['value'] = $update_type;
        $logging = node_submit($logging);
        node_save($logging);
        if ($logging->nid) {
            $data['result'] = $logging->nid;
        } else {
            $data['result'] = false;
        }
    }else{
        $data['result'] = false;
    }
    return $data;
}
function _logging_service_create($mac,$mac_eth,$data){
    global $user;


    if($mac && $mac_eth && $data) {
        //Check current device logs is created or not
        $query = db_select('taxonomy_term_data', 'ttd');
        $query->fields('ttd', array('tid'));
        $query->fields('lv',array('field_launcher_version_value'));
        $query->leftJoin('field_data_field_launcher_version','lv','lv.entity_id=ttd.tid');
        $db_or = db_or();
        $db_or->condition('ttd.name', $mac);
        $db_or->condition('ttd.name', $mac_eth);
        $query->condition($db_or);
        $result_term = $query->execute()->fetchAssoc();
        if($result_term['tid']) {
            $query = db_select('node', 'n');
            $query->fields('n', array('nid'));
            $start_date = date('Y-m-d 00:00:00');
            $end_date = date('Y-m-d 23:59:59');
            $query->condition('n.type', 'device_logs');
            $query->condition('n.created', strtotime($start_date), '>=');
            $query->condition('n.created', strtotime($end_date), '<=');
            $log_result = $query->execute()->fetchAssoc();
            if ($log_result['nid']) {
                $logging = node_load($log_result['nid']);
                $logging->field_data_logs['und'][0]['value'] = $data;
                node_save($logging);
                $result['result'] = $log_result['nid'];
            } else {
                $logging = new stdClass();
                $logging->type = 'device_logs';
                $logging->title = 'Logs : '.$mac_eth.' Date : '.date('Y-m-d');
                $logging->status = 1;
                $logging->language = LANGUAGE_NONE;
                $logging->uid = 1;
                $logging->field_data_logs['und'][0]['value'] = $data;
                $logging->field_device_log_mac['und']['0']['value'] = $mac_eth;
                $logging = node_submit($logging);
                node_save($logging);
                if ($logging->nid) {
                    $result['result'] = $logging->nid;
                } else {
                    $result['result'] = 'false';
                }
            }
        }else{
            $result['result'] = 'false';
        }
    }else{
        $result['result'] = 'false';
    }


    return $result;
}

function _playlist_completed_service_create($mac,$playlist_id){
    global $user;
    if($mac && $playlist_id) {
        $logging = new stdClass();
        $logging->type = 'playlist_download_logs';
        $logging->title = $mac;
        $logging->status = 1;
        $logging->language = 'en';
        $logging->uid = 1;
        $logging->field_downloaded_playlist_id['und'][0]['value'] = $playlist_id;
        $logging = node_submit($logging);
        node_save($logging);
        if ($logging->nid) {
            $data['result'] = $logging->nid;
        } else {
            $data['result'] = false;
        }
    }else{
        $data['result'] = false;
    }
    return $data;
}

/**
 * Access callback for the note service.
 *
 * @param string $op
 *  The operation that's going to be performed.
 * @param array $args
 *  The arguments that will be passed to the callback.
 * @return bool
 *  Whether access is given or not.
 */

function ip() {
    if(!empty($_SERVER['HTTP_CLIENT_IP']))
    {
        $ip = $_SERVER['HTTP_CLIENT_IP'];
    }
    elseif(!empty($_SERVER['HTTP_X_FORWARDED_FOR']))
    {
        $ip = $_SERVER['HTTP_X_FORWARDED_FOR'];
    }
    else
    {
        $ip = $_SERVER['REMOTE_ADDR'];
    };
    return $ip;
}

function _playlist_index($mac,$mac_eth,$complete_videos,$playlist_id,$broadcast_id) {

    global $user;
    $data = array();
    $playlist_welcome_id = 549;
    $playlist_default_id = 564;
//    $playlist_welcome_id = 116;
//    $playlist_default_id = 8233;
    //Load photo holder
    $photo_holder_query = db_select('node','n');
    $photo_holder_query->fields('n',array('nid'));
    $photo_holder_query->condition('n.type','photo_holder');
    $photo_holder_query->condition('n.status',1);
    $photo_holder_query->range(0,1);
    $photo_holder_result = $photo_holder_query->execute()->fetchAssoc();
    if($photo_holder_result['nid']){
        $photo_holder = node_load($photo_holder_result['nid']);
        $photo_holder_url = file_create_url($photo_holder->field_file_photo_holder['und']['0']['uri']);
    }
    if($mac || $mac_eth) {

        $query = db_select('taxonomy_term_data', 'ttd');
        $query->fields('ttd', array('tid'));
        //$query->condition('ttd.name', $mac);
        $db_or = db_or();
        $db_or->condition('ttd.name',$mac);
        $db_or->condition('ttd.name',$mac_eth);
        $query->condition($db_or);
        $result = $query->execute()->fetchAssoc();

        if ($result) {
            //Update MAC status
            $mac_term = taxonomy_term_load($result['tid']);
            $device_company_id = $mac_term->field_mac_company_id['und'][0]['tid'];
            $device_region_id = $mac_term->field_mac_region_id['und'][0]['tid'];
            $device_province_id = $mac_term->field_mac_province_id['und'][0]['tid'];
            $device_store_id = $mac_term->field_mac_store_id['und'][0]['tid'];
            $is_public_internet = isset($mac_term->field_public_internet['und'][0]['value']) ? $mac_term->field_public_internet['und'][0]['value'] : 0;

            $mac_term->field_mac_last_login['und'][0]['value'] = strtotime(date("Y-m-d H:i:s", time()));
            $mac_term->field_mac_ip['und'][0]['value'] = ip();
            taxonomy_term_save($mac_term);



            $str_playlist = '';
            //Check and update to logging broadcast & playlist
            if(isset($broadcast_id) && $broadcast_id) {
                $logging_query = db_select('node', 'n');
                $logging_query->fields('n', array('nid'));
                $logging_query->fields('lpi', array('field_logging_playlist_id_value'));
                $logging_query->leftJoin('field_data_field_logging_mac', 'lm', 'lm.entity_id=n.nid');
                $logging_query->condition('field_logging_mac_value', $mac_eth);
                $logging_query->leftJoin('field_data_field_logging_broadcast_id', 'lbi', 'lbi.entity_id=n.nid');
                $logging_query->condition('field_logging_broadcast_id_nid', $broadcast_id);
                $logging_query->leftJoin('field_data_field_logging_playlist_id', 'lpi', 'lpi.entity_id=n.nid');
                //$logging_query->leftJoin('field_data_field_logging_date','ld','ld.entity_id=n.nid');
                //$logging_query->condition('field_logging_date_value',strtotime(date('Y-m-d',time())));
                $logging_result = $logging_query->execute()->fetchAssoc();

                if ($logging_result['nid']) {
                    $logging = node_load($logging_result['nid']);
                    $playlists = json_decode($logging_result['field_logging_playlist_id_value'], true);

//                    $playlists[] = array(
//                        'playlist_id' => $playlist_id,
//                        'broadcast_id' => $broadcast_id,
//                        'time_is_playing' => strtotime(date('Y-m-d H:i:s'))
//                    );

                    //$str_playlist = json_encode($playlists);
                    //$logging->field_logging_playlist_id['und'][0]['value'] = $str_playlist;
                    $logging->field_logging_downloaded['und'][0]['value'] = $complete_videos;
                    node_save($logging);
                } else {
//                    $playlists[] = array(
//                        'playlist_id' => $playlist_id,
//                        'broadcast_id' => $broadcast_id,
//                        'time_is_playing' => strtotime(date('Y-m-d H:i:s'))
//                    );
//                    $str_playlist = json_encode($playlists);
                    //T?o m?i b?n ghi logging theo ngày
                    $logging = new stdClass();
                    $logging->type = 'logging_broadcast_playlist';
                    $logging->title = 'Logging Broadcast Playlist ' . $mac_eth . '(' . date('Y-m-d') . ')';
                    $logging->language = LANGUAGE_NONE;
                    $logging->uid = 1;
                    $logging->field_logging_broadcast_id['und'][0]['nid'] = $broadcast_id;
                    $logging->field_logging_mac['und'][0]['value'] = $mac_eth;
//                    $logging->field_logging_playlist_id['und'][0]['value'] = $str_playlist;
                    $logging->field_logging_downloaded['und'][0]['value'] = $complete_videos;
                    $logging->field_logging_date['und'][0]['value'] = strtotime(date('Y-m-d H:i:s'));
                    node_save($logging);
                }
            }


            //L?y l?ch phát sóng

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



            if($broadcast_current_id) {

                $query = db_select('node', 'n');
                $query->fields('n',array('nid'));
                $query->condition('n.type', 'broadcast_playlist_time');
                $query->leftJoin('field_data_field_broadcast_playlist_time_id', 'bpt', 'bpt.entity_id=n.nid');
                $query->condition('field_broadcast_playlist_time_id_nid', $broadcast_current_id);
                $playlist_result = $query->execute();

                if ($playlist_result->rowCount()) {
                    $i = 0;
                    $count = 1;


                    foreach ($playlist_result as $broadcast_time_id) {

                        $broadcast_time = node_load($broadcast_time_id->nid);
                        //Load Playlist info
                        $playlist_for_play = node_load($broadcast_time->field_playlist_broadcast_time_id['und'][0]['nid']);

                        $data[$i]['id'] = $playlist_for_play->nid;
                        $data[$i]['description'] = '';
                        $data[$i]['created'] = $playlist_for_play->created;
                        $data[$i]['modified'] = $playlist_for_play->changed;
                        $start_time = (string)strtotime(date('Y-m-d',$current_date).' '.date('H:i:s',$broadcast_time->field_broadcast_playlist_start['und'][0]['value']));
                        $end_time = (string)strtotime(date('Y-m-d',$current_date).' '.date('H:i:s',$broadcast_time->field_broadcast_playlist_end['und'][0]['value']));
                        $data[$i]['date_start_time'] = $start_time;
                        $data[$i]['date_end_time'] = $end_time;


                        $data[$i]['date_start_time_text'] = date('Y-m-d H:i:s',$start_time);
                        $data[$i]['date_end_time_text'] = date('Y-m-d H:i:s',$end_time);
                        $data[$i]['current_time'] = time();
                        $data[$i]['current_time_text'] = date('Y-m-d H:i:s',time());
                        $data[$i]['broadcast_id'] = $broadcast_time->field_broadcast_playlist_time_id['und'][0]['nid'];
                        $data[$i]['playlist_type'] = $playlist_for_play->field_playlist_default['und'][0]['value'];
                        $data[$i]['mac'] = $mac;

                        if (isset($playlist_for_play->field_slides_playlist['und']) && $playlist_for_play->field_slides_playlist['und']) {
                            $slides = $playlist_for_play->field_slides_playlist['und'];

                            $orders = isset($playlist_for_play->field_playlist_orders['und'][0]['value']) ? json_decode($playlist_for_play->field_playlist_orders['und'][0]['value']) : 0;
                            if($orders){
                                foreach($orders as $key=>$value){
                                    $order_slides[$key] = $value;
                                }
                            }
                            $k = 0;
                            if ($slides) {
                                foreach ($slides as $nid) {
                                    $slide = node_load($nid['nid']);

                                    $data[$i]['playlist'][$k]['id'] = $slide->nid;
                                    $data[$i]['playlist'][$k]['description'] = '';
                                    $data[$i]['playlist'][$k]['type'] = $slide->field_media_type['und']['0']['value'];
                                    $data[$i]['playlist'][$k]['sortorder'] = isset($order_slides[$slide->nid]) ? $order_slides[$slide->nid] : $count;
                                    if(isset($slide->field_link_open_file['und'][0]['value'])){
                                        if($is_public_internet==2){
                                            $data[$i]['playlist'][$k]['video_url'] = str_replace('cdnmkt','cdnmkt2',$slide->field_link_open_file['und'][0]['value']);
                                        }else{
                                            $data[$i]['playlist'][$k]['video_url'] = $slide->field_link_open_file['und'][0]['value'];
                                        }
                                    }else{
                                        $data[$i]['playlist'][$k]['video_url'] = file_create_url(($slide->field_slide_file['und']['0']['uri']));
                                    }

                                    $data[$i]['playlist'][$k]['livestreaming'] = isset($slide->field_livestream_url['und'][0]['value']) ? $slide->field_livestream_url['und'][0]['value'] : '';
                                    $start = $slide->field_slide_start_time;
                                    //if ($start) {
                                    $data[$i]['playlist'][$k]['schedule'] = 'limited';
                                    $data[$i]['playlist'][$k]['start'] = isset($start['und'][0]['value']) ? $start['und'][0]['value'] : 0;
                                    if($slide->field_media_type['und']['0']['value']==3){
                                        $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value']*1000;
                                    }else{
                                        $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value'];
                                    }

                                    $data[$i]['playlist'][$k]['filesize'] = $slide->field_file_size['und']['0']['value'];

                                    if($slide->field_is_static_slide['und']['0']['value']==2){
                                        $data[$i]['playlist'][$k]['is_static'] = 0;
                                    }else{
                                        $data[$i]['playlist'][$k]['is_static'] = 1;
                                    }
                                    $data[$i]['playlist'][$k]['photo_holder'] = $photo_holder_url;
                                    $data[$i]['playlist'][$k]['created'] = $slide->created;
                                    $data[$i]['playlist'][$k]['modified'] = $slide->changed;
                                    $k++;
                                    $count++;

                                }
                            }
                            //}
                            $i++;
                        }
                    }
                    //Add playlist welcome
                    $playlist_id = $playlist_welcome_id;
                    //cmsmkt
                    //$playlist_id = 116;
                    $playlist = node_load($playlist_id);
                    $data[$i]['id'] = $playlist->nid;
                    $data[$i]['description'] = '';
                    $data[$i]['created'] = $playlist->created;
                    $data[$i]['modified'] = $playlist->changed;
                    $start_time = strtotime(date('Y-m-d 01:00:00'));
                    $end_time = strtotime(date('Y-m-d 01:05:00'));
                    $data[$i]['date_start_time'] = $start_time;
                    $data[$i]['date_end_time'] = $end_time;
                    $data[$i]['date_start_time_text'] = date('Y-m-d H:i:s',$start_time);
                    $data[$i]['date_end_time_text'] = date('Y-m-d H:i:s',$end_time);
                    $data[$i]['current_time'] = time();
                    $data[$i]['current_time_text'] = date('Y-m-d H:i:s',time());
                    $data[$i]['broadcast_id'] = $broadcast_current_id;
                    $data[$i]['playlist_type'] = $playlist->field_playlist_default['und'][0]['value'];
                    $data[$i]['mac'] = $mac;
                    if ($playlist->field_slides_playlist) {
                        $slides = $playlist->field_slides_playlist['und'];
                        $orders = isset($playlist->field_playlist_orders['und'][0]['value']) ? json_decode($playlist->field_playlist_orders['und'][0]['value']) : 0;
                        if($orders){
                            foreach($orders as $key=>$value){
                                $order_slides[$key] = $value;
                            }
                        }
                        $k = 0;
                        $count = 1;
                        if ($slides) {
                            foreach ($slides as $nid) {
                                $slide = node_load($nid['nid']);

                                $data[$i]['playlist'][$k]['id'] = $slide->nid;
                                $data[$i]['playlist'][$k]['description'] = $slide->body['und'][0]['value'];
                                $data[$i]['playlist'][$k]['type'] = $slide->field_media_type['und']['0']['value'];
                                $data[$i]['playlist'][$k]['sortorder'] = isset($order_slides[$slide->nid]) ? $order_slides[$slide->nid] : $count;
                                if(isset($slide->field_link_open_file['und'][0]['value'])){
                                    if($is_public_internet==2){
                                        $data[$i]['playlist'][$k]['video_url'] = str_replace('cdnmkt','cdnmkt2',$slide->field_link_open_file['und'][0]['value']);
                                    }else{
                                        $data[$i]['playlist'][$k]['video_url'] = $slide->field_link_open_file['und'][0]['value'];
                                    }
                                }else{
                                    $data[$i]['playlist'][$k]['video_url'] = file_create_url(($slide->field_slide_file['und']['0']['uri']));
                                }
                                $data[$i]['playlist'][$k]['livestreaming'] = $slide->field_livestream_url['und'][0]['value'];
                                $start = $slide->field_slide_start_time;
                                //if ($start) {
                                $data[$i]['playlist'][$k]['schedule'] = 'limited';
                                $data[$i]['playlist'][$k]['start'] = $start['und'][0]['value'];
                                if($slide->field_media_type['und']['0']['value']==3){
                                    $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value']*1000;
                                }else{
                                    $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value'];
                                }

                                $data[$i]['playlist'][$k]['filesize'] = $slide->field_file_size['und']['0']['value'];

                                if($slide->field_is_static_slide['und']['0']['value']==2){
                                    $data[$i]['playlist'][$k]['is_static'] = 0;
                                }else{
                                    $data[$i]['playlist'][$k]['is_static'] = 1;
                                }
                                $data[$i]['playlist'][$k]['photo_holder'] = $photo_holder_url;
                                $data[$i]['playlist'][$k]['created'] = $slide->created;
                                $data[$i]['playlist'][$k]['modified'] = $slide->changed;
                                $k++;
                                $count++;

                            }
                        }
                    }
                }
            }else{
                //Load Playlist Default
                $i = 0;
                $count = 1;

                $playlist_for_play = node_load($playlist_default_id);

                $data[$i]['id'] = $playlist_for_play->nid;
                $data[$i]['description'] = '';
                $data[$i]['created'] = $playlist_for_play->created;
                $data[$i]['modified'] = $playlist_for_play->changed;
                $start_time = strtotime(date('Y-m-d 07:00:00'));
                $end_time = strtotime(date('Y-m-d 23:59:00'));
                $data[$i]['date_start_time'] = $start_time;
                $data[$i]['date_end_time'] = $end_time;
                $data[$i]['date_start_time_text'] = date('Y-m-d H:i:s',$start_time);
                $data[$i]['date_end_time_text'] = date('Y-m-d H:i:s',$end_time);
                $data[$i]['current_time'] = time();
                $data[$i]['current_time_text'] = date('Y-m-d H:i:s',time());
                $data[$i]['broadcast_id'] = $playlist_default_id;
                $data[$i]['playlist_type'] = $playlist_for_play->field_playlist_default['und'][0]['value'];
                $data[$i]['mac'] = $mac;

                if (isset($playlist_for_play->field_slides_playlist['und']) && $playlist_for_play->field_slides_playlist['und']) {
                    $slides = $playlist_for_play->field_slides_playlist['und'];
                    $orders = isset($playlist_for_play->field_playlist_orders['und'][0]['value']) ? json_decode($playlist_for_play->field_playlist_orders['und'][0]['value']) : 0;
                    if($orders){
                        foreach($orders as $key=>$value){
                            $order_slides[$key] = $value;
                        }
                    }

                    $k = 0;
                    if ($slides) {
                        foreach ($slides as $nid) {
                            $slide = node_load($nid['nid']);

                            $data[$i]['playlist'][$k]['id'] = $slide->nid;
                            $data[$i]['playlist'][$k]['description'] = '';
                            $data[$i]['playlist'][$k]['type'] = $slide->field_media_type['und']['0']['value'];
                            $data[$i]['playlist'][$k]['sortorder'] = isset($order_slides[$slide->nid]) ? $order_slides[$slide->nid] : $count;
                            if(isset($slide->field_link_open_file['und'][0]['value'])){
                                if($is_public_internet==2){
                                    $data[$i]['playlist'][$k]['video_url'] = str_replace('cdnmkt','cdnmkt2',$slide->field_link_open_file['und'][0]['value']);
                                }else{
                                    $data[$i]['playlist'][$k]['video_url'] = $slide->field_link_open_file['und'][0]['value'];
                                }
                            }else{
                                $data[$i]['playlist'][$k]['video_url'] = file_create_url(($slide->field_slide_file['und']['0']['uri']));
                            }
                            $data[$i]['playlist'][$k]['livestreaming'] = isset($slide->field_livestream_url['und'][0]['value']) ? $slide->field_livestream_url['und'][0]['value'] : '';
                            $start = $slide->field_slide_start_time;
                            //if ($start) {
                            $data[$i]['playlist'][$k]['schedule'] = 'limited';
                            $data[$i]['playlist'][$k]['start'] = isset($start['und'][0]['value']) ? $start['und'][0]['value'] : 0;
                            if($slide->field_media_type['und']['0']['value']==3){
                                $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value']*1000;
                            }else{
                                $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value'];
                            }

                            $data[$i]['playlist'][$k]['filesize'] = $slide->field_file_size['und']['0']['value'];

                            if($slide->field_is_static_slide['und']['0']['value']==2){
                                $data[$i]['playlist'][$k]['is_static'] = 0;
                            }else{
                                $data[$i]['playlist'][$k]['is_static'] = 1;
                            }
                            $data[$i]['playlist'][$k]['photo_holder'] = $photo_holder_url;
                            $data[$i]['playlist'][$k]['created'] = $slide->created;
                            $data[$i]['playlist'][$k]['modified'] = $slide->changed;
                            $k++;
                            $count++;
                        }
                    }
                }
            }
        } else {
            //update new MAC
            $voca = taxonomy_vocabulary_machine_name_load('mac_address');
            $new_mac = new stdClass();
            $new_mac->name = $mac_eth;
            $new_mac->vid = $voca->vid;
            $new_mac->parent = 0;
            $new_mac->field_mac_last_login['und'][0]['value'] = strtotime(date("Y-m-d H:i:s", time()));
            $new_mac->field_mac_ip['und'][0]['value'] = ip();
            $new_mac->field_mac_company_id['und'][0]['tid'] = 100;
            taxonomy_term_save($new_mac);
            //Return the connected succesfull

            $playlist_id = $playlist_welcome_id;
            //cmsmkt
            //$playlist_id = 116;
            $playlist = node_load($playlist_id);
            $i = 0;
            $data[$i]['id'] = $playlist_id;
            $data[$i]['description'] = '';
            $data[$i]['created'] = $playlist->created;
            $data[$i]['modified'] = $playlist->changed;
            $start_time = strtotime(date('Y-m-d 07:00:00'));
            $end_time = strtotime(date('Y-m-d 23:59:00'));
            $data[$i]['date_start_time'] = $start_time;
            $data[$i]['date_end_time'] = $end_time;
            $data[$i]['date_start_time_text'] = date('Y-m-d H:i:s',$start_time);
            $data[$i]['date_end_time_text'] = date('Y-m-d H:i:s',$end_time);
            $data[$i]['current_time'] = time();
            $data[$i]['current_time_text'] = date('Y-m-d H:i:s',time());
            $data[$i]['broadcast_id'] = $playlist_id;
            $data[$i]['playlist_type'] = $playlist->field_playlist_default['und'][0]['value'];
            $data[$i]['mac'] = $mac;
            if ($playlist->field_slides_playlist) {
                $slides = $playlist->field_slides_playlist['und'];
                $orders = isset($playlist->field_playlist_orders['und'][0]['value']) ? json_decode($playlist->field_playlist_orders['und'][0]['value']) : 0;
                if($orders){
                    foreach($orders as $key=>$value){
                        $order_slides[$key] = $value;
                    }
                }
                $k = 0;
                $count = 1;
                if ($slides) {
                    foreach ($slides as $nid) {
                        $slide = node_load($nid['nid']);

                        $data[$i]['playlist'][$k]['id'] = $slide->nid;
                        $data[$i]['playlist'][$k]['description'] = $slide->body['und'][0]['value'];
                        $data[$i]['playlist'][$k]['type'] = $slide->field_media_type['und']['0']['value'];
                        $data[$i]['playlist'][$k]['sortorder'] = isset($order_slides[$slide->nid]) ? $order_slides[$slide->nid] : $count;
                        if(isset($slide->field_link_open_file['und'][0]['value'])){
                            $data[$i]['playlist'][$k]['video_url'] = $slide->field_link_open_file['und'][0]['value'];
                        }else{
                            $data[$i]['playlist'][$k]['video_url'] = file_create_url(($slide->field_slide_file['und']['0']['uri']));
                        }
                        $data[$i]['playlist'][$k]['livestreaming'] = $slide->field_livestream_url['und'][0]['value'];
                        $start = $slide->field_slide_start_time;
                        //if ($start) {
                        $data[$i]['playlist'][$k]['schedule'] = 'limited';
                        $data[$i]['playlist'][$k]['start'] = $start['und'][0]['value'];
                        if($slide->field_media_type['und']['0']['value']==3){
                            $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value']*1000;
                        }else{
                            $data[$i]['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value'];
                        }

                        $data[$i]['playlist'][$k]['filesize'] = $slide->field_file_size['und']['0']['value'];

                        if($slide->field_is_static_slide['und']['0']['value']==2){
                            $data[$i]['playlist'][$k]['is_static'] = 0;
                        }else{
                            $data[$i]['playlist'][$k]['is_static'] = 1;
                        }
                        $data[$i]['playlist'][$k]['photo_holder'] = $photo_holder_url;
                        $data[$i]['playlist'][$k]['created'] = $slide->created;
                        $data[$i]['playlist'][$k]['modified'] = $slide->changed;
                        $k++;
                        $count++;

                    }
                }
            }
        }
    }else{
        $data['result'] = false;
    }
    return $data;

}


?>
