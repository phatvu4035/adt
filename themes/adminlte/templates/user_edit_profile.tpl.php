<?php 
global $user;
?>
<form action="<?php print $form['#action']; ?>" method="<?php print $form['#method']; ?>" id="<?php print $form['#id']; ?>" accept-charset="UTF-8">
<?php print drupal_render($form['form_build_id']); ?>
<?php print drupal_render($form['form_token']); ?>
<?php print drupal_render($form['form_id']); ?>
<div class="row">
	<div class="col-md-3">
		<div class="box box-primary">
			<div class="box-body box-profile">
				<?php print render($form['picture']); ?>
				<ul class="list-group list-group-unbordered">
					<li class="list-group-item">
						<strong>Vai trò: </strong><?php 
						  $roles = $user->roles;
						  unset($roles[2]);
						  print implode(',', $roles);
						?>
					</li>
				</ul>
			</div>
			
		</div>		
	</div>
	<div class="col-md-9">
		<div class="nav-tabs-custom user-form-custom box box-primary">
			<div class="tab-content">
				<div class="form-group">
					<div class="col-md-2">
						<label>Tên đăng nhập<span class="form-required" title="This field is required.">*</span></label>
					</div>
					<div class="col-md-10 form-custom-input">
						<?php print render($form['account']['name']) ?>
					</div>
				</div>
				<div class="form-group">
					<div class="col-md-2">
						<label>Email <span class="form-required" title="This field is required.">*</span></label>
					</div>
					<div class="col-md-10 form-custom-input">
						<?php print render($form['account']['mail']) ?>
					</div>
				</div>

				<div class="form-group">
					<div class="col-md-2">
						<label>Mật khẩu mới</label>
					</div>
					<div class="col-md-10 form-custom-input">
						<?php print render($form['account']['pass']); ?>
					</div>
				</div>
				
				<div class="form-group">
					<div class="col-md-2">
						<label>Mật khẩu hiện tại</label>
					</div>
					<div class="col-md-10 form-custom-input">
						<?php print render($form['account']['current_pass']); ?>
					</div>
				</div>


				<div class="form-group">
					<div class="col-md-offset-2 col-md-10">
						<?php print drupal_render_children($form['actions']); ?>
					</div>
				</div>
			</div>
		</div>
		
	</div>

</div>

</form> 


