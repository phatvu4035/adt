<?php

// Xay dung form create user
function adtrue_manage_edit_user($form,&$form_state,$value) {
	
}

function adtrue_manage_view_user($form,&$form_state,$value) {
    global $base_url;
	$form = array();
	$user_obj = user_load($value);
	$form['user_profile'] = array(
		'#type' => 'container',
        '#attributes' => array(
            'class' => array('row')
        ),
	);
    $form['user_profile']['col-left'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="col-md-3">',
        '#suffix' => '</div>',
    );
    $form['user_profile']['col-right'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="col-md-9">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('box','box-primary')
        ),
    );
	$form['user_profile']['col-left']['user_picture'] = array(
		'#type' => 'container',
        '#prefix' => '<div class="box box-primary">',
        '#suffix' => '</div>',
        '#attributes' => array(
            'class' => array('box-body','box-profile')
        ),
	);
    $user_url = isset($user_obj->picture->uri) ? file_create_url($user_obj->picture->uri) : '/sites/default/files/admin.png';
    $form['user_profile']['col-left']['user_picture']['picture'] = array(
        '#type' => 'item',
        '#markup' => '<img class="profile-user-img img-responsive img-circle" src="/sites/default/files/admin.png" alt="User profile picture">
            <h3 class="profile-username text-center">'.$user_obj->name.'</h3>'
    );
    $roles = $user_obj->roles;
    unset($roles[2]);
    $roles_list = implode(',', $roles);
    $form['user_profile']['col-left']['user_picture']['rol_user'] = array(
        '#type' => 'item',
        '#markup' =>'<ul class="list-group list-group-unbordered">
                     <li class="list-group-item">
                        <strong>Vai trò: </strong>'.$roles_list.'
                     </li>
                     </ul>',
    );
    $form['user_profile']['col-right']['user_info'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('tab-content')
        ),
        '#prefix' => '<div class="nav-tabs-custom">',
        '#suffix' => '</div>',
    );

    $form['user_profile']['col-right']['user_info']['name'] = array(
        '#type' => 'item',
        '#markup' => '<div class="form-group">
                    <label class="col-sm-3 control-label">Name: </label>

                    <div class="col-sm-9">
                      <p>'.$user_obj->name.'</p>
                    </div>
                  </div>',
    );
    $form['user_profile']['col-right']['user_info']['email'] = array(
        '#type' => 'item',
        '#markup' => '<div class="form-group">
                    <label class="col-sm-3 control-label">Email: </label>
                    <div class="col-sm-9">
                      <p>'.$user_obj->mail.'</p>
                    </div>
                  </div>',
    );
    $company_id = (isset($user_obj->field_user_company_id['und'][0]['tid'])) ? $user_obj->field_user_company_id['und'][0]['tid'] : 0;
    if($company_id) {
        $company = taxonomy_term_load($company_id);
        $form['user_profile']['col-right']['user_info']['company'] = array(
            '#type' => 'item',
            '#markup' => '<div class="form-group">
                <label class="col-sm-3 control-label">Công ty: </label>
                <div class="col-sm-9">
                  <p>'.$company->name.'</p>
                </div>
               </div>',
        );
    }
    
    $region_id = (isset($user_obj->field_user_region_id['und'][0]['tid'])) ? $user_obj->field_user_region_id['und'][0]['tid'] : 0;
    if($region_id) {
        $region = taxonomy_term_load($region_id);
        $form['user_profile']['col-right']['user_info']['region'] = array(
            '#type' => 'item',
            '#markup' => '<div class="form-group">
                <label class="col-sm-3 control-label">Vùng miền: </label>
                <div class="col-sm-9">
                  <p>'.$region->name.'</p>
                </div>
               </div>',
        );
    }
    
    $province_id = (isset($user_obj->field_user_province_id['und'][0]['tid'])) ? $user_obj->field_user_province_id['und'][0]['tid'] : 0;
    if($province_id) {
        $province = taxonomy_term_load($province_id);
        $form['user_profile']['col-right']['user_info']['province'] = array(
            '#type' => 'item',
            '#markup' => '<div class="form-group">
                <label class="col-sm-3 control-label">Tỉnh thành:</label>
                <div class="col-sm-9">
                  <p>'.$province->name.'</p>
                </div>
               </div>',   
        );
    }
    $form['user_profile']['col-right']['user_info']['updated'] = array(
        '#type' => 'item',
        '#markup' => '<div class="form-group">
                    <label class="col-sm-3 control-label">Cập nhật lần cuối:</label>

                    <div class="col-sm-9">
                      <p>'.date('H:i:s d/m/Y',$user_obj->login).'</p>
                    </div>
                  </div>',
    );
    $form['user_profile']['col-right']['user_info']['edit_profile'] = array(
        '#type' => 'item',
        '#markup' =>'<a href="'.$base_url.'/user/'.$user_obj->uid.'/edit" class="btn btn-success pull-right"><i class="fa fa-edit"></i>Cập nhật thông tin:</a>',
        '#weight' => 4,
        '#prefix' => '<div class="box-search-item">',
        '#suffix' => '</div>',
    );
    
    return $form;
}