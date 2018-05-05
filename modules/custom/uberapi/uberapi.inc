<?php
// note_service.inc
/**
 * Callback for creating note services.
 *
 * @param object $data
 * @return object

function _note_service_create($data) {
    global $user;

    unset($data->id);
    $data->uid = $user->uid;
    $data->created = time();
    $data->modified = time();

    if (!isset($data->subject)) {
        return services_error('Missing note attribute subject', 406);
    }

    if (!isset($data->note)) {
        return services_error('Missing note attribute note', 406);
    }

    note_service_write_note($data);

    return (object)array(
        'id' => $data->id,
        'uri' => services_service_uri(array('note', $data->id)),
    );
}
 */
// note_service.inc
/**
 * Callback for updating note services.
 *
 * @param int $id
 * @param object $data
 * @return object

function _note_service_update($id, $data) {
    global $user;
    $note = note_service_get_note($id);

    unset($data->created);
    $data->id = $id;
    $data->uid = $note->uid;
    $data->modified = time();

    note_service_write_note($data);

    return (object)array(
        'id' => $id,
        'uri' => services_service_uri(array('note', $id)),
    );
}
/**
 * Callback for retrieving note services.
 *
 * @param int $id
 * @return object

function _playlist_retrieve($date) {
    return playlist_get_playlist($date);
}

/**
 * Callback for deleting note services.
 *
 * @param int $id
 * @return object

function _note_service_delete($id) {
    note_service_delete_note($id);

    return (object)array(
        'id' => $id,
    );
}
 */

function _uberapi_index() {
    global $user;
    //if($user->uid) {
        $playlist = array();
        $query = db_select('node', 'n');
        $query->fields('n', array('nid'));
        $query->leftJoin('field_data_field_slide_order', 'fso', 'fso.entity_id=n.nid');
        $query->condition('n.type', 'uber_ad');
        $query->condition('n.status', 1);
        //$query->orderBy('n.changed','DESC');
        $query->orderBy('fso.field_slide_order_value', 'DESC');

        $result = $query->execute();
        if ($result->rowCount()) {
            $i = 0;

            foreach ($result as $row) {
                $playlist = node_load(17);


                $data['id'] = $playlist->nid;
                $data['description'] = $playlist->body['und'][0]['value'];
                $data['created'] = $playlist->created;
                $data['modified'] = $playlist->changed;

                if ($playlist->field_uberad_slides) {
                    $slides = $playlist->field_uberad_slides['und'];
                    $k = 0;
                    if ($slides) {
                        foreach ($slides as $nid) {
                            $slide = node_load($nid['nid']);

                            $data['playlist'][$k]['id'] = $slide->nid;
                            $data['playlist'][$k]['type'] = $slide->field_media_type['und']['0']['value'];
                            $data['playlist'][$k]['sortorder'] = $slide->field_slide_order['und']['0']['value'];
                            $data['playlist'][$k]['video_url'] = file_create_url(($slide->field_slide_file['und']['0']['uri']));
                            //$data['playlist'][$k]['video_url'] = 'http://cms.megaview.vn/bo_y_te_28_05_720p_new.mp4';
                            $start = $slide->field_slide_start_time;
                            if ($start) {
                                $data['playlist'][$k]['schedule'] = 'limited';
                                $data['playlist'][$k]['start'] = $start['und'][0]['value'];
                                $data['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value'];
                            } else {
                                $data['playlist'][$k]['schedule'] = 'unlimited';
                                $data['playlist'][$k]['start'] = 0;
                                $data['playlist'][$k]['duration'] = 0;
                            }
                            $data['playlist'][$k]['duration'] = $slide->field_slide_duration['und'][0]['value'];
                            $data['playlist'][$k]['filetime'] = $slide->field_file_time['und']['0']['value'];
                            $data['playlist'][$k]['created'] = $slide->created;
                            $data['playlist'][$k]['modified'] = $slide->changed;

                            $ads_query = db_select('node', 'n');
                            $ads_query->fields('n', array('nid'));
                            $ads_query->leftJoin('field_data_field_ads_slide', 'fas', 'fas.entity_id=n.nid');
                            $ads_query->condition('fas.field_ads_slide_nid', $nid['nid']);
                            $ads_query->condition('n.type', 'advertisement');
                            $ads_query->condition('n.status', 1);
                            $ads_result = $ads_query->execute();
                            $ads = array();
                            if ($ads_result->rowCount()) {
                                $t = 0;
                                foreach ($ads_result as $item) {
                                    $ads = node_load($item->nid);
                                    $data['playlist'][$k]['ads'][$t]['ads_id'] = $ads->nid;
                                    $data['playlist'][$k]['ads'][$t]['type'] = $ads->field_ads_type['und'][0]['value'];
                                    $data['playlist'][$k]['ads'][$t]['start'] = $ads->field_ads_start['und'][0]['value'];

                                    if (is_null($ads->field_ads_content['und'][0]['value'])) {
                                        $data['playlist'][$k]['ads'][$t]['content'] = file_create_url($ads->field_ads_file['und'][0]['uri']);
                                    } else {
                                        $data['playlist'][$k]['ads'][$t]['content'] = $ads->field_ads_content['und'][0]['value'];
                                    }
                                    $data['playlist'][$k]['ads'][$t]['duration'] = $ads->field_ads_duration['und'][0]['value'];

                                    $data['playlist'][$k]['ads'][$t]['position'] = $ads->field_ads_position['und'][0]['value'];
                                    $t++;
                                }
                            }
                            $k++;
                        }
                    }
                }
                $i++;
            }
        }
    
    
    return $data;
}
/*
function playlist_get_playlist($date) {
    $query = db_select('node','n');
    $query->fields('n',array('nid','title'));
    $query->fields('fbd',array('field_broadcast_date_value'));
    $query->leftJoin('field_data_field_broadcast_date','fbd','fbd.entity_id=n.nid');
    $query->condition('n.type','playlist');
    $query->condition('fbd.field_broadcast_date_value',$date,'>=');
    $result = $query->execute();
    $data = array();
    if($result->rowCount()){
        $i = 0;
        foreach($result as $row){
            $data[$i]['id'] = $row->nid;
            $data[$i]['title'] = $row->title;

            $i++;
        }
    }

    return $data;


}*/

?>