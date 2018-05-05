<?php 

/*
* View diary mac device
*/
function mac_check_history_log($form,$form_state,$value) {
	global $base_url;
    $result = load_diary_object($value);
    $rows = $result['rows'];

    $mac = taxonomy_term_load($value);

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

    $form['loggin_info']['header']['create_note'] = array(
        '#type' => 'item',
        '#markup' => '<a href="'.$base_url.'/mac/check/history/'.$value.'/add" class="btn btn-info pull-right"><i class="fa fa-plus"></i> Thêm nhật kí</a>',
    );
    $form['loggin_info']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );

    $form['loggin_info']['body']['current_mac'] = array(
        '#type' => 'item',
        '#markup' => '<b>MAC : </b>' . $mac->name,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $first_date = '';
    if(isset($mac->field_date_created_at['und'][0]['value'])) {
        $first_date = date('H:i:s d-m-Y',$mac->field_date_created_at['und'][0]['value']);
    }
    $form['loggin_info']['body']['first_diary_date'] = array(
        '#type' => 'item',
        '#markup' => '<b>Ngày gán thiết bị: </b>' . $first_date,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $first_user_name = '';
    if(isset($mac->field_mac_person_add['und'][0]['uid'])) {
        $first_user_name = user_load($mac->field_mac_person_add['und'][0]['uid']);
        $first_user_name = $first_user_name->name;
    }
    
    $form['loggin_info']['body']['first_diary_user'] = array(
        '#type' => 'item',
        '#markup' => '<b>Người gán: </b>' . $first_user_name,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $header = array(
        array('data' => 'Người tạo'),
        array('data' => 'Ngày tạo'),
        array('data' => 'SAP'),
        array('data' => 'Nguyên nhân'),
        array('data' => 'Hướng xử lí'),
        array('data' => 'Trạng thái'),
        array('data' => 'Ngày cập nhật'),
        array('data' => 'Ghi chú'),
        array('data' => 'Thao tác'),
    );
    $form['list_log'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-success')
        ),
    );
    $form['list_log']['header'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['list_log']['header']['header_title'] = array(
        '#type' => 'item',
        '#weight' => 2,
        '#markup' => '<h3>Nhật kí thiết bị</h3>',
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['list_log']['body'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#prefix' => '<div class="box-body with-border">',
        '#suffix' => '</div>'
    );
    

    $form['list_log']['body']['result'] = array(
        '#theme' => 'table',
        '#header' => $header,
        '#rows' => $rows,
        '#attributes' => array('class'=>array('table table-bordered')),
        '#empty' => t('Chưa có nhật kí nào'),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    return $form;
}

function load_diary_object($mac_id) {
    global $user,$base_url;
	$number_per_page = 20;

	$query = db_select('node','n')->extend('PagerDefault')->distinct()->limit($number_per_page);
	$query->condition('n.type','history_check_mac');
	$query->leftJoin('field_data_field_mac_check','macid','macid.entity_id = n.nid');
	$query->condition('macid.field_mac_check_tid',$mac_id);

	// Get MAC id
	$query->leftJoin('taxonomy_term_data','ttd','ttd.tid = macid.field_mac_check_tid');

	$query->fields('n',array('nid'));
	$query->fields('ttd',array('name'));
	$query->orderBy('n.nid','DESC');
	$query_result = $query->execute();
	//
	$count = 1;
	$mac = '';
	$rows = array();
	$result = array();

    $status = array(
        1 => 'Mới',
        2 => 'Đang xử lí',
        3 => 'Hoàn thành',
        4 => 'Tạm dừng',
        5 => 'Hủy'
    );

	if( $query_result->rowCount() ) {
		foreach($query_result as $row) {
			$node = node_load($row->nid);
			$node_user = user_load($node->uid);

            $mac_id = isset($node->field_mac_check['und'][0]['tid']) ? $node->field_mac_check['und'][0]['tid'] : 0;
            $mac_tax = taxonomy_term_load($mac_id);
            $store_id = isset($mac_tax->field_mac_store_id['und'][0]['tid']) ? $mac_tax->field_mac_store_id['und'][0]['tid'] : 0;
            $store = taxonomy_term_load($store_id);

            $store_sap = '';
            if(isset($store->field_store_sap['und'][0]['value'])) {
                $store_sap = '<a href="'.$base_url.'/mac/list?get=1&store_name='.$store->field_store_sap['und'][0]['value'].'">'.$store->field_store_sap['und'][0]['value'].'</a>';
            }

            $status_value = '';
            if(isset($node->field_field_mac_checking_status['und'][0]['value']) ) {
                $status_value = $status[$node->field_field_mac_checking_status['und'][0]['value']];
            }

            $edit = '';
            if($node_user->uid !== $user->uid) {
                $edit = '';
            } else {
                $edit = '<div class="btn-group">
                              <button type="button" class="btn btn-success">Thao tác</button>
                              <button type="button" class="btn btn-success dropdown-toggle" data-toggle="dropdown">
                                  <span class="caret"></span>
                                  <span class="sr-only">Toggle Dropdown</span>
                              </button>
                              <ul class="dropdown-menu" role="menu">
                                  <li><a href="'.$base_url.'/mac/check/history/log/'.$node->nid.'/edit" ><i class="fa fa-edit"></i> Sửa</a></li>
                                  <li><a href="'.$base_url.'/mac/check/history/log/'.$node->nid.'/delete" ><i class="fa fa-trash"></i> Xóa</a></li>
                              </ul>
                          </div>';
            }

			$rows[] = array(
				$node_user->name,
				date('H:i:s d-m-Y',$node->created),
                $store_sap,
				isset($node->field_mac_check_reason['und'][0]['value']) ? $node->field_mac_check_reason['und'][0]['value'] : '',
				isset($node->field_handle_check_mac['und'][0]['value']) ? $node->field_handle_check_mac['und'][0]['value'] : '',
				$status_value,
				date('H:i:s d-m-Y',$node->changed),
				isset($node->field_mac_check_note['und'][0]['value']) ? $node->field_mac_check_note['und'][0]['value'] : '',
                $edit,
			);
		}
	}
	$result['rows'] = $rows;
	return $result;
}

function history_log_form($node = '',$mac_id = '') {
    $form['update_diary'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box box-success">',
        '#suffix' => '</div>'
    );
    $form['update_diary']['header'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['update_diary']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['update_diary']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );

    $form['update_diary']['header']['header_title'] = array(
        '#type' => 'item',
        '#markup' => '<h3>Cập nhật nhật kí</h3>',
        '#prefix' => '<div class="col-md-10">',
        '#suffix' => '</div>'
    );
    // body
    $form['update_diary']['body']['nid'] = array(
        '#type' => 'hidden',
        '#default_value' => isset($node->nid) ? $node->nid : '',
    );
    $form['update_diary']['body']['mac_id'] = array(
        '#type' => 'hidden',
        '#default_value' => $mac_id,
    );
    $form['update_diary']['body']['check_reason'] = array(
        '#type' => 'textarea',
        '#title' => 'Nguyên nhân',
        '#rows' => 4,
        '#default_value' => isset($node->field_mac_check_reason['und'][0]['value']) ? $node->field_mac_check_reason['und'][0]['value'] : '',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>'
    );
    $form['update_diary']['body']['solve_way'] = array(
        '#type' => 'textarea',
        '#rows' => 4,
        '#title' => 'Hướng giải quyết',
        '#default_value' => isset($node->field_handle_check_mac['und'][0]['value']) ? $node->field_handle_check_mac['und'][0]['value'] : '',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>' 
    );
    $form['update_diary']['body']['check_note'] = array(
        '#type' => 'textarea',
        '#rows' => 4,
        '#title' => 'Ghi chú',
        '#default_value' => isset($node->field_mac_check_note['und'][0]['value']) ? $node->field_mac_check_note['und'][0]['value'] : '',
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>' 
    );
    $status = array(
        1 => 'Mới',
        2 => 'Đang xử lí',
        3 => 'Hoàn thành',
        4 => 'Tạm dừng',
        5 => 'Hủy'
    );
    $form['update_diary']['body']['check_status'] = array(
        '#type' => 'select',
        '#title' => 'Trạng thái ',
        '#default_value' => isset($node->field_field_mac_checking_status['und'][0]['value']) ? $node->field_field_mac_checking_status['und'][0]['value'] : 1,
        '#options' => $status,
        '#prefix' => '<div class="col-md-6">',
        '#suffix' => '</div>' 
    );

    // footer
    $form['update_diary']['footer']['submit'] = array(
        '#type' => 'submit',
        '#weight' => 6,
        '#value' => t('Cập nhật'),
        '#attributtes' => array('class'=>array('btn','btn-info')),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>'
    );

    return $form;
}

function mac_check_history_log_add($form,$form_state,$value) {
    $form = history_log_form('',$value);
    return $form;
}

function mac_check_history_log_edit($form,$form_state,$value) {
    global $user;
	$node = node_load($value);
    $uid = $node->uid;
    if($user->uid !== $uid) {
        drupal_set_message('Bạn không thể sủa nội dung này','error');
        drupal_exit();
    }

    $mac_id = $node->field_mac_check['und'][0]['tid'];

    $form = history_log_form($node,$mac_id);
    return $form;
}

function mac_check_history_log_delete($form,$form_state,$value) {
    global $user;
    global $base_url;
    $node = node_load($value);
    $uid = $node->uid;
    $node_user = user_load($uid);

    if($user->uid !== $uid) {
        drupal_set_message('Bạn không thể sủa nội dung này','error');
        drupal_exit();
    }

    $form['loggin_info'] = array(
        '#type' => 'container',
        '#weight' => 1,
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

    $mac_id = $node->field_mac_check['und'][0]['tid'];
    $mac = taxonomy_term_load($mac_id);

    $form['loggin_info']['header']['back_note'] = array(
        '#type' => 'item',
        '#markup' => '<a href="'.$base_url.'/mac/check/history/'.$mac_id.'" class="btn btn-info pull-right"><i class="fa fa-reply"></i>Quay lại</a>',
    );
    $form['loggin_info']['header']['current_mac'] = array(
        '#type' => 'item',
        '#markup' => '<b>MAC: </b>' . $mac->name,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['header']['first_diary_date'] = array(
        '#type' => 'item',
        '#markup' => '<b>Ngày gán thiết bị: </b>' . date('d-m-Y',$node->created),
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $form['loggin_info']['header']['first_diary_user'] = array(
        '#type' => 'item',
        '#markup' => '<b>Người gán: </b>' . $node_user->name,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    // body
    $form['update_diary'] = array(
        '#type' => 'container',
        '#weight' => 2,
        '#attributes' => array(
            'class' => array('box','box-info')
        ),
    );
    $form['update_diary']['body'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['update_diary']['body']['nid'] = array(
        '#type' => 'hidden',
        '#default_value' => $node->nid,
    );
    $form['update_diary']['body']['mac_id'] = array(
        '#type' => 'hidden',
        '#default_value' => $mac->tid,
    );
    $check_reason = isset($node->field_mac_check_reason['und'][0]['value']) ? $node->field_mac_check_reason['und'][0]['value'] : '';
    $form['update_diary']['body']['check_reason'] = array(
        '#type' => 'item',
        '#markup' => '<b>Nguyên nhân: </b>'.$check_reason,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $solve_way = isset($node->field_handle_check_mac['und'][0]['value']) ? $node->field_handle_check_mac['und'][0]['value'] : '';
    $form['update_diary']['body']['solve_way'] = array(
        '#type' => 'item',
        '#markup' => '<b>Hướng giải quyết: </b>'.$solve_way,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );
    $check_note = isset($node->field_mac_check_note['und'][0]['value']) ? $node->field_mac_check_note['und'][0]['value'] : '';
    $form['update_diary']['body']['check_note'] = array(
        '#type' => 'item',
        '#markup' => '<b>Ghi chú: </b>'.$check_note,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    $status = array(
        1 => 'Mới',
        2 => 'Đang xử lí',
        3 => 'Hoàn thành',
        4 => 'Tạm dừng',
        5 => 'Hủy'
    );
    $status_value = '';
    if(isset($node->field_field_mac_checking_status['und'][0]['value']) ) {
        $status_value = $status[$node->field_field_mac_checking_status['und'][0]['value']];
    }

    $form['update_diary']['body']['check_status'] = array(
        '#type' => 'item',
        '#markup' => '<b>Trạng thái: </b>'.$status_value,
        '#prefix' => '<div class="col-md-12">',
        '#suffix' => '</div>'
    );

    // footer
    $form['update_diary']['footer'] = array(
        '#type' => 'container',
        '#prefix' => '<div class="box-header with-border">',
        '#suffix' => '</div>'
    );
    $form['update_diary']['footer']['submit'] = array(
        '#type' => 'submit',
        '#weight' => 6,
        '#value' => t('Xóa'),
        '#attributtes' => array('class'=>array('btn','btn-info')),
        '#prefix' => '<div class="col-md-2 pull-right">',
        '#suffix' => '</div>'
    );

    return $form;
}


function mac_check_history_log_edit_submit($form,&$form_state) {
    global $base_url;

    $check_reason = $form_state['values']['check_reason'];
    $solve_way = $form_state['values']['solve_way'];
    $check_note = $form_state['values']['check_note'];
    $check_status = $form_state['values']['check_status'];
    $nid = $form_state['values']['nid'];
    $mac_id = $form_state['values']['mac_id'];

    $node = node_load($nid);
    $node->field_mac_check_reason['und'][0]['value'] = $check_reason;
    $node->field_handle_check_mac['und'][0]['value'] = $solve_way;
    $node->field_mac_check_note['und'][0]['value'] = $check_note;
    $node->field_field_mac_checking_status['und'][0]['value'] = $check_status;


    // Save the node
    node_save($node);
    drupal_set_message('Đã cập nhật nhật kí thiết bị');
    $form_state['redirect'] = $base_url.'/mac/check/history/'.$mac_id;
}

// Add cap nhat thiet bi
function mac_check_history_log_add_submit($form,&$form_state) {
    global $user;
    global $base_url;
    $check_reason = $form_state['values']['check_reason'];
    $solve_way = $form_state['values']['solve_way'];
    $check_note = $form_state['values']['check_note'];
    $check_status = $form_state['values']['check_status'];
    $nid = $form_state['values']['nid'];
    $mac_id = $form_state['values']['mac_id'];

    $mac = taxonomy_term_load($mac_id);
    $cur_time = time();

    $node = new stdClass();
    $node->title = $mac->name;
    $node->uid = $user->uid;
    $node->type = 'history_check_mac';
    $node->status = 1;
    $node->field_mac_check['und'][0]['tid'] = $mac_id;
    $node->field_mac_attach_date['und'][0]['value'] = $cur_time;
    $node->field_mac_check_person['und'][0]['uid'] = $user->uid;
    $node->field_mac_check_reason['und'][0]['value'] = $check_reason;
    $node->field_handle_check_mac['und'][0]['value'] = $solve_way;
    $node->field_field_mac_checking_status['und'][0]['value'] = $check_status;
    $node->field_mac_check_note['und'][0]['value'] = $check_note;

    node_save($node);
    drupal_set_message('Cập nhật nhật kí thành công');
    $form_state['redirect'] = $base_url.'/mac/check/history/'.$mac_id;

}

function mac_check_history_log_delete_submit($form,&$form_state) {
    global $base_url;

    $nid = $form_state['values']['nid'];
    $mac_id = $form_state['values']['mac_id'];

    node_delete($nid);
    drupal_set_message('Đã xóa một nhật kí');
    $form_state['redirect'] = $base_url.'/mac/check/history/'.$mac_id;
}