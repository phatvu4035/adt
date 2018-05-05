<?php
function _apps_index() {
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->condition('n.type','apps');
    $query->condition('n.status',1);
    $query->orderBy('n.created','DESC');
    $query->range(0,1);
    $result = $query->execute()->fetchAssoc();
    $data = array();
    if($result){
        $app = node_load($result['nid']);
        $data['filename'] = file_create_url(($app->field_filename['und']['0']['uri']));
        $data['version_code'] = $app->field_version_code['und']['0']['value'];
        $data['version_name'] = $app->field_version_name['und']['0']['value'];
        $data['created'] = $app->created;
        $data['updated'] = $app->changed;
    }

    return $data;

}

function _get_update_launcher(){
    $query = db_select('node','n');
    $query->fields('n',array('nid'));
    $query->leftJoin('field_data_field_app_type','fat','fat.entity_id=n.nid');
    $query->condition('n.type','apps');
    $query->condition('fat.field_app_type_value',1);
    $query->condition('n.status',1);
    $query->range(0,1);
    $result = $query->execute()->fetchAssoc();
    $data = array();
    if($result){
        $app = node_load($result['nid']);
        $data['filename'] = file_create_url(($app->field_filename['und']['0']['uri']));
        $data['version_code'] = $app->field_version_code['und']['0']['value'];
        $data['version_name'] = $app->field_version_name['und']['0']['value'];
        $data['created'] = $app->created;
        $data['updated'] = $app->changed;
    }

    return $data;
}




?>