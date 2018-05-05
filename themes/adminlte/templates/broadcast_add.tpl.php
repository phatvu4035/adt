<div class="wrap_podcast">
	<div class="box_podcast">
		<div class="property">
			<div class="col-md-3">
				<?php print render($form['podcast_property']['company_id']); ?>
			</div>
			<div class="col-md-2">
				<?php print render($form['podcast_property']['region_id']); ?>
			</div>
			<div class="col-md-2">
				<?php print render($form['podcast_property']['province_id']); ?>
			</div>
			<div class="col-md-5">
				<?php print render($form['podcast_property']['store_id']); ?>
			</div>
			<div class="col-md-8 name_broadcast">
				<?php print render($form['podcast_name']); ?>
			</div>
		</div>

		<div class="grid_playlist">
			<?php print render($form['podcast_grid_playlist']); ?>
		</div>
		<div class="row capnhat_epg">
			<div class="col-md-2 pull-right">
				<?php print render($form['podcast_additional']['submit']); ?>	
			</div>
		</div>
		<div class="danhsach_epg">
			<?php print drupal_render_children($form); ?>
		</div>
	</div>
</div>
