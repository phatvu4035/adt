<?php

function load_region_by_company($form,$form_state) {
   $company_id = $form_state['values']['company_id'];
   $matches = filter_region_by_company($company_id);
   
   $form['fieldset_all']['fieldset_property']['body']['region_id']['#options'] = $matches;
   return $form['fieldset_all']['fieldset_property']['body']['region_id'];
}
// Edit store
function match_region_by_company_store($form,$form_state) {
    $company_id = $form_state['values']['company_id'];
    $matches = filter_region_by_company($company_id);
   
    $form['fieldset_all']['body']['field_info']['region_id']['#options'] = $matches;
    return $form['fieldset_all']['body']['field_info']['region_id'];
}

function load_province_by_region($form,$form_state) {
    $region_id = $form_state['values']['region_id'];
    $matches = filter_province_by_region($region_id);
    $form['fieldset_all']['fieldset_property']['province_id']['#options'] = $matches;
    return $form['fieldset_all']['fieldset_property']['province_id'];
}
// Edit store
function match_province_by_region_store($form,$form_state) {
    $region_id = $form_state['values']['region_id'];
    $matches = filter_province_by_region($region_id);
    $form['fieldset_all']['body']['field_info']['province_id']['#options'] = $matches;
    return $form['fieldset_all']['body']['field_info']['province_id'];
}

function province_load_region_by_company_table($form,$form_state) {
    $company_id = $form_state['values']['company'];
     
    $matches = filter_region_by_company($company_id);
    $form['danhsach_province']['header_filter']['region']['#options'] = $matches;
    $form['danhsach_province']['header_filter']['region']['#prefix'] = '<div id="region-wrapper">';
    return $form['danhsach_province']['header_filter']['region'];
}

function store_load_region_by_company_table($form,$form_state) {
    $company_id = $form_state['values']['company'];  
    $matches = filter_region_by_company($company_id);

    $form['danhsach_store']['header_filter']['region']['#options'] = $matches;
    $form['danhsach_store']['header_filter']['region']['#prefix'] = '<div id="region-wrapper">';
    return $form['danhsach_store']['header_filter']['region'];
}

/*
* Import store 
*/
function store_import_load_region_by_company($form,$form_state) {
    $company_id = $form_state['values']['company_id'];
    $matches = filter_region_by_company($company_id);
    $form['upload_store']['header']['header_filter']['region_id']['#options'] = $matches;
    $form['upload_store']['header']['header_filter']['region_id']['#prefix'] = '<div id="region-wrapper">';
    return $form['upload_store']['header']['header_filter']['region_id'];
}
function store_import_load_province_by_region($form,$form_state) {
    $region_id = $form_state['values']['region_id'];
    $matches = filter_province_by_region($region_id);
    $form['upload_store']['header']['header_filter']['province_id']['#options'] = $matches;
    $form['upload_store']['header']['header_filter']['province_id']['#prefix'] = '<div id="province-wrapper">';
    return $form['upload_store']['header']['header_filter']['province_id'];
}

function store_load_province_by_region_table($form,$form_state) {
    $region_id = $form_state['values']['region'];
    
    $matches = filter_province_by_region($region_id);
    $form['danhsach_store']['header_filter']['province']['#options'] = $matches;
    $form['danhsach_store']['header_filter']['province']['#prefix'] = '<div id="province-wrapper">';
    return $form['danhsach_store']['header_filter']['province'];
}
