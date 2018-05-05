<?php
function manage_user_list_form($form,$form_state){
    $form = array();
    $company_id = isset($_GET['company_id']) ? $_GET['company_id'] : 0;
    $region_id = isset($_GET['region_id']) ? $_GET['region_id'] : 0;
    $province_id = isset($_GET['province_id']) ? $_GET['province_id'] : 0;
    $store_id = isset($_GET['store_id']) ? $_GET['store_id'] : 0;
    $page = isset($_GET['page']) ? $_GET['page'] : 0;

    $header =  array(
        array('data'=>'STT'),
        array('data'=>'Tên truy cập'),
        array('data'=>'Quyền'),
        array('data' => 'Công ty'),
        array('data' => 'Vùng miền'),
        array('data' => 'Tỉnh/Thành'),
        array('data'=>'Thao tác'),

    );

    $result = list_users($company_id,$region_id,$province_id,$store_id,$page);
    $rows = $result['rows'];
    $total_result = $result['total_result'];


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
        '#markup' =>'<h3>Danh sách người dùng</h3>',
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
        '#prefix' => '<div class="col-md-4">',
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
        '#prefix' => '<div id="region-wrapper" class="col-md-4">',
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
        '#prefix' => '<div id="province-wrapper" class="col-md-4">',
        '#suffix' => '</div>',
        '#validated' => TRUE,
        '#ajax' => array(
            'callback' => 'store_select_callback',
            'wrapper' => 'store-wrapper',
            'method' => 'replace',
            'effect' => 'fade',
        ),
    );

    $form['list_broadcast']['body']['buttons'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'

    );
    $form['list_broadcast']['body']['buttons']['search_button'] = array(
        '#type' => 'submit',
        '#value' => 'Tìm kiếm',
        '#weight' => 3,
        '#prefix' => '<div class="col-md-3 pull-right">',
        '#suffix' => '</div>'

    );
    $message = '<h4>Tổng số người dùng : '.$total_result.'</h4>';

    $form['list_broadcast']['body']['search_summary'] = array(
        '#type' => 'item',
        '#weight' => 2,
        '#markup' => '<div class="callout callout-info">'.$message.'</div>',
        '#prefix' => '<div class="col-md-12" style="margin-top : 30px;">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['body']['result'] = array(
        '#weight' => 3,
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#empty' => 'Chưa có Nội dung nào',
        '#attributes' => array('class'=>'table table-striped'),
        '#prefix' => '<div class="box-body">',
        '#suffix' => '</div>'
    );
    $form['list_broadcast']['pager'] = array(
        '#theme' => 'pager',
        '#weight' => 4
    );

    return $form;

}

function manage_user_list_form_submit($form,&$form_state){
    global $base_url;
    $values = $form_state['values'];

    $company_id = isset($values['broadcast_company']) ? $values['broadcast_company'] : 0;
    $region_id = isset($values['broadcast_region']) ? $values['broadcast_region'] : 0;
    $province_id = isset($values['broadcast_province']) ? $values['broadcast_province'] : 0;

    $filter = array(
        'company_id' => $company_id,
        'region_id' => $region_id,
        'province_id' => $province_id,
    );

    $query_string = http_build_query($filter,'','&');
    $form_state['redirect'] = $base_url.'/manage/user/list?'.$query_string;
}

function list_users($company_id=0,$region_id=0,$province_id=0,$store_id=0,$page=0,$number_per_page=50){
    global $base_url,$user;
    $query = db_select('users', 'u')->extend('PagerDefault')->distinct()->limit($number_per_page);
    $query->groupBy('u.uid');
    $query->fields('u',array('uid','name'));
    $query->condition('u.uid',0,'<>');

    $query->fields('uc',array('field_user_company_id_tid'));
    $query->fields('ur',array('field_user_region_id_tid'));
    $query->fields('up',array('field_user_province_id_tid'));


    $query->leftJoin('field_data_field_user_company_id','uc','uc.entity_id=u.uid');
    $query->leftJoin('field_data_field_user_region_id','ur','ur.entity_id=u.uid');
    $query->leftJoin('field_data_field_user_province_id','up','up.entity_id=u.uid');

    //filter by search submit
    if($company_id){
        $query->condition('field_user_company_id_tid',$company_id);
    }
    if($region_id){
        $query->condition('field_user_region_id_tid',array('tid'=>$region_id),'IN');
    }
    if($province_id){
        $query->condition('field_user_province_id_tid',array('tid'=>$province_id),'IN');
    }
    
    $total_result = $query->countQuery()->execute()->fetchField();
    $user_result = $query->execute();
    $rows = array();
    $count = ($page*$number_per_page) + 1;
    if($user_result->rowCount()){

        foreach ($user_result as $row) {
            $user_info = user_load($row->uid);
            $str_roles = '';
            $str_company_role = '';
            $str_region_role = '';
            $str_province_role = '';
            $str_store_role = '';
            //Show list user roles
            if($user_info->roles) {
                foreach ($user_info->roles as $key => $role) {
                    $str_roles .= $role . '<br/>';
                }
            }else{
                $str_roles = '';
            }
            //Show company role
            if(isset($user_info->field_user_company_id['und'])){
                foreach($user_info->field_user_company_id['und'] as $key=>$company_tid){
                    $company_terms = taxonomy_term_load($company_tid['tid']);
                    $str_company_role .= $company_terms->name.'<br/>';
                }
            }
            //Show region role
            if(isset($user_info->field_user_region_id['und'])){
                foreach($user_info->field_user_region_id['und'] as $key=>$region_tid){
                    $region_terms = taxonomy_term_load($region_tid['tid']);
                    $str_region_role .= $region_terms->name.'<br/>';
                }
            }
            //Show province role
            if(isset($user_info->field_user_province_id['und'])){
                foreach($user_info->field_user_province_id['und'] as $key=>$province_tid){
                    $province_terms = taxonomy_term_load($province_tid['tid']);
                    $str_province_role .= $province_terms->name.'<br/>';
                }
            }
            //Show store role
            if(isset($user_info->field_user_store_id['und'])){
                foreach($user_info->field_user_store_id['und'] as $key=>$store_tid){
                    $store_terms = taxonomy_term_load($store_tid['tid']);
                    $str_store_role .= $store_terms->name.'<br/>';
                }
            }
            $rows[] = array(
                $count,
                $user_info->name,
                $str_roles,
                $str_company_role,
                $str_region_role,
                $str_province_role,
                $str_store_role,
                '<div class="btn-group">
                    <button type="button" class="btn btn-success">Thao tác</button>
                    <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                        <span class="caret"></span>
                        <span class="sr-only">Toggle Dropdown</span>
                    </button>
                    <ul class="dropdown-menu" role="menu">
                        <li><a href="'.$base_url.'/manage/user/edit/'.$user_info->uid.'" ><i class="fa fa-sign-in"></i> Phân quyền</a></li>
                        <li><a href="'.$base_url.'/manage/user/reset/'.$user_info->uid.'" ><i class="fa fa-mail-reply-all"></i> Reset</a></li>
                    </ul>
                </div>'
            );
            $count++;
        };
    }
    $result['rows']=$rows;
    $result['total_result'] = $total_result;
    return $result;
}

function manage_user_edit_form($form,&$form_state,$uid){
    global $base_url;
    if($uid){
        $user_info = user_load($uid);
        $company_id = 0;
        $region_id = 0;
        $province_id = 0;

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
            '#markup' =>'<h3>Phân quyền người dùng</h3>',
        );
        $form['list_broadcast']['body'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="box-body with-border">',
            '#suffix' => '</div>'
        );
        $str_roles = '';
        $str_company_role = '';
        $str_region_role = '';
        $str_province_role = '';
        $str_store_role = '';
        //Show list user roles
        if($user_info->roles) {
            foreach ($user_info->roles as $key => $role) {
                $str_roles .= '<div>'.$role . '</div>';
            }
        }else{
            $str_roles = '';
        }
        //Show company role
        if(isset($user_info->field_user_company_id['und'])){
            foreach($user_info->field_user_company_id['und'] as $key=>$company_tid){
                $company_terms = taxonomy_term_load($company_tid['tid']);
                $str_company_role .= '<div>'.$company_terms->name.'</div>';
            }
        }
        //Show region role
        if(isset($user_info->field_user_region_id['und'])){
            foreach($user_info->field_user_region_id['und'] as $key=>$region_tid){
                $region_terms = taxonomy_term_load($region_tid['tid']);
                $str_region_role .= '<div>'.$region_terms->name.'</div>';
            }
        }
        //Show province role
        if(isset($user_info->field_user_province_id['und'])){
            foreach($user_info->field_user_province_id['und'] as $key=>$province_tid){
                $province_terms = taxonomy_term_load($province_tid['tid']);
                $str_province_role .= '<div>'.$province_terms->name.'</div>';
            }
        }
        //Show store role
        if(isset($user_info->field_user_store_id['und'])){
            foreach($user_info->field_user_store_id['und'] as $key=>$store_tid){
                $store_terms = taxonomy_term_load($store_tid['tid']);
                $str_store_role .= '<div>'.$store_terms->name.'</div>';
            }
        }

        $message = '
            <div class="callout callout-info">
                <h4>Các quyền đang có của người dùng : '.$user_info->name.'</h4>
                
                <p><b>Chức năng</b>'.$str_roles.'</p>
                <p><b>Nội dung</b>'.$str_company_role.$str_region_role.$str_province_role.'</p>
                
            </div>';
        $form['list_broadcast']['body']['message'] = array(
            '#type' => 'item',
            '#weight' => 2,
            '#markup' => $message,
            '#prefix' => '<div class="col-md-12">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['body']['broadcast_area'] = array(
            '#type' => 'container',
            '#weight' => 2,
        );

        $company_options = load_voca_info('company');
        $form['list_broadcast']['body']['broadcast_area']['broadcast_company'] = array(
            '#type' => 'select',
            '#title' => 'Công ty',
            '#options' => $company_options,
            #default_value' => $company_id,
            '#prefix' => '<div class="col-md-4">',
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
            '#prefix' => '<div id="region-wrapper" class="col-md-4">',
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
            '#prefix' => '<div id="province-wrapper" class="col-md-4">',
            '#suffix' => '</div>',
            '#validated' => TRUE,
            '#ajax' => array(
                'callback' => 'store_select_callback',
                'wrapper' => 'store-wrapper',
                'method' => 'replace',
                'effect' => 'fade',
            ),
        );
        $form['list_broadcast']['footer'] = array(
            '#type' => 'container',
            '#weight' => 12,
            '#prefix' => '<div class="box-footer with-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['user_id'] = array(
            '#type' => 'hidden',
            '#value' => $user_info->uid
        );
        $form['list_broadcast']['footer']['back'] = array(
            '#type' => 'item',
            '#weight' => 2,
            '#markup' => '<a href="'.$base_url.'/manage/user/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>'
        );
        $form['list_broadcast']['footer']['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Cấp quyền',
            '#attributes' => array(
                'class' => array('btn','btn-success')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>'
        );
        return $form;
    }else{
        drupal_goto($base_url.'/manage/user/list');
    }
}

function manage_user_edit_form_submit($form,&$form_state){
    global $base_url;
    $values = $form_state['values'];
    $uid = $values['user_id'];
    $user = user_load($uid);

    if($values['broadcast_company']){
        $user->field_user_company_id['und'][0]['tid'] = $values['broadcast_company'];
    }
    if($values['broadcast_region']){
        $region_tids = isset($user->field_user_region_id['und']) ? $user->field_user_region_id['und'] : 0;
        $region_existed = 0;
        if($region_tids) {
            foreach ($region_tids as $key => $region_tid) {
                if ($region_tid['tid'] == $values['broadcast_region']) {
                    $region_existed = 1;
                    break;
                }
            }
        }
        if(!$region_existed){
            $user->field_user_region_id['und'][] = array('tid' => $values['broadcast_region']);
        }
    }


    if($values['broadcast_province']){
        $province_tids = isset($user->field_user_province_id['und']) ? $user->field_user_province_id['und'] : 0;
        $province_existed = 0;
        if($province_tids){
            foreach($province_tids as $key=>$province_tid){
                if($province_tid['tid'] == $values['broadcast_province']){
                    $province_existed = 1;
                }
            }
        }
        if(!$province_existed){
            $user->field_user_province_id['und'][] = array('tid' => $values['broadcast_province']);
        }

    }
    user_save($user);
    $form_state['redirect'] = $base_url.'/manage/user/list';
    drupal_set_message('Cập nhật phân quyền thành công!');

}

function manage_user_reset_form($form,&$form_state,$uid){
    global $base_url;
    if($uid){
        $user_info = user_load($uid);


        $form['list_broadcast'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#attributes' => array(
                'class' => array('box','box-danger')
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
            '#markup' =>'<h3>Reset quyền người dùng</h3>',
        );
        $form['list_broadcast']['body'] = array(
            '#type' => 'container',
            '#weight' => 2,
            '#prefix' => '<div class="box-body with-border">',
            '#suffix' => '</div>'
        );
        $str_roles = '';
        $str_company_role = '';
        $str_region_role = '';
        $str_province_role = '';
        $str_store_role = '';
        //Show list user roles
        if($user_info->roles) {
            foreach ($user_info->roles as $key => $role) {
                $str_roles .= '<div>'.$role . '</div>';
            }
        }else{
            $str_roles = '';
        }
        //Show company role
        if(isset($user_info->field_user_company_id['und'])){
            foreach($user_info->field_user_company_id['und'] as $key=>$company_tid){
                $company_terms = taxonomy_term_load($company_tid['tid']);
                $str_company_role .= '<div>'.$company_terms->name.'</div>';
            }
        }
        //Show region role
        if(isset($user_info->field_user_region_id['und'])){
            foreach($user_info->field_user_region_id['und'] as $key=>$region_tid){
                $region_terms = taxonomy_term_load($region_tid['tid']);
                $str_region_role .= '<div>'.$region_terms->name.'</div>';
            }
        }
        //Show province role
        if(isset($user_info->field_user_province_id['und'])){
            foreach($user_info->field_user_province_id['und'] as $key=>$province_tid){
                $province_terms = taxonomy_term_load($province_tid['tid']);
                $str_province_role .= '<div>'.$province_terms->name.'</div>';
            }
        }
        //Show store role
        if(isset($user_info->field_user_store_id['und'])){
            foreach($user_info->field_user_store_id['und'] as $key=>$store_tid){
                $store_terms = taxonomy_term_load($store_tid['tid']);
                $str_store_role .= '<div>'.$store_terms->name.'</div>';
            }
        }
        $header =  array(
            array('data'=>'STT'),
            array('data'=>'Tên truy cập'),
            array('data'=>'Quyền'),
            array('data' => 'Công ty'),
            array('data' => 'Vùng miền'),
            array('data' => 'Tỉnh/Thành'),

        );

        $rows[] = array(
            1,
            $user_info->name,
            $str_roles,
            $str_company_role,
            $str_region_role,
            $str_province_role,
            $str_store_role
        );


        $form['list_broadcast']['body']['result'] = array(
            '#weight' => 3,
            '#theme' => 'table',
            '#header' => $header,
            '#rows' => $rows,
            '#empty' => 'Chưa có Nội dung nào',
            '#attributes' => array('class'=>'table table-striped'),
            '#prefix' => '<div class="box-body">',
            '#suffix' => '</div>'
        );

        $form['list_broadcast']['footer'] = array(
            '#type' => 'container',
            '#weight' => 12,
            '#prefix' => '<div class="box-footer with-border">',
            '#suffix' => '</div>'
        );
        $form['list_broadcast']['user_id'] = array(
            '#type' => 'hidden',
            '#value' => $user_info->uid
        );
        $form['list_broadcast']['footer']['back'] = array(
            '#type' => 'item',
            '#weight' => 2,
            '#markup' => '<a href="'.$base_url.'/manage/user/list" class="btn btn-info pull-right"><i class="fa fa-reply"></i> Quay lại</a>'
        );
        $form['list_broadcast']['footer']['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Reset quyền',
            '#attributes' => array(
                'class' => array('btn','btn-danger')
            ),
            '#prefix' => '<div class="col-md-2 pull-right">',
            '#suffix' => '</div>'
        );
        return $form;
    }else{
        drupal_goto($base_url.'/manage/user/list');
    }
}
function manage_user_reset_form_submit($form,&$form_state){
    global $base_url;
    $uid = $form_state['values']['user_id'];
    $user = user_load($uid);
    $user->field_user_company_id['und'] = array();
    $user->field_user_region_id['und'] = array();
    $user->field_user_province_id['und'] = array();
    user_save($user);
    $form_state['redirect'] = $base_url.'/manage/user/list';
    drupal_set_message('Reset quyền người dùng thành công!');
}

?>