<div class="col-md-12 wr">
	<div class="box box-success">
		<div class="form-wrapper">
			<div class="col-md-3">
				<?php print render($form['company_id']); ?>
			</div>
			<div class="col-md-3">
				<?php print render($form['region_id']); ?>
			</div>
			<div class="col-md-3">
				<?php print render($form['province_id']); ?>
			</div>
			<div class="col-md-3">
				<?php print render($form['autocomplete_store_string_container']); ?>
			</div>
		</div>
		<div class="form-wrapper next_row">
			<div class="col-md-3">
				<?php print render($form['mac_term']); ?>
			</div>
			<div class="col-md-3">
				<?php print render($form['mac_ip']); ?>
			</div>
			<div class="col-md-3">
				<?php print render($form['mac_trangthai']); ?>
			</div>
			<div class="col-md-3">
				<?php print render($form['public_internet']); ?>
			</div>
		</div>
		<div class="form-wrapper submit_mac">
			<div class="col-md-2 pull-right">
				<?php print render($form['submit']); ?>
			</div>
		</div>
		<div class="noidungrest form-wrapper">
			<?php print drupal_render_children($form); ?>
		</div>
	</div>
</div>