<?php
function playlist_broadcast_record($form,$form_state) {
	global $user;
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


}