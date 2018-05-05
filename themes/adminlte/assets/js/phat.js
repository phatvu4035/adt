jQuery(document).ready(function() {

	// function sortTable() {
	// 	  var table, rows, switching, i, x, y, shouldSwitch;
	// 	  table = document.getElementById("myTable");
	// 	  switching = true;
	// 	  /* Make a loop that will continue until
	// 	  no switching has been done: */
	// 	  while (switching) {
	// 	    // Start by saying: no switching is done:
	// 	    switching = false;
	// 	    rows = table.getElementsByTagName("TR");
	// 	    /* Loop through all table rows (except the
	// 	    first, which contains table headers): */
	// 	    for (i = 1; i < (rows.length - 1); i++) {
	// 	      // Start by saying there should be no switching:
	// 	      shouldSwitch = false;
	// 	       Get the two elements you want to compare,
	// 	      one from current row and one from the next: 
	// 	      x = rows[i].getElementsByTagName("TD")[0];
	// 	      y = rows[i + 1].getElementsByTagName("TD")[0];
	// 	      // Check if the two rows should switch place:
	// 	      if (x.innerHTML.toLowerCase() > y.innerHTML.toLowerCase()) {
	// 	        // I so, mark as a switch and break the loop:
	// 	        shouldSwitch= true;
	// 	        break;
	// 	      }
	// 	    }
	// 	    if (shouldSwitch) {
	// 	      /* If a switch has been marked, make the switch
	// 	      and mark that a switch has been done: */
	// 	      rows[i].parentNode.insertBefore(rows[i + 1], rows[i]);
	// 	      switching = true;
	// 	    }
	// 	 }
	// }

    // Broadcast
	// Trigger when company field is selected
	var company = jQuery('.form-item-company select').val();
	var region  = jQuery('#edit-header-filter .form-item-region select').val();
	var province = jQuery('#edit-header-filter .form-item-province select').val();
	var store = jQuery('#edit-header-filter .form-item-store select').val();

	var href_broadcast = jQuery('#edit-create-broadcast a').attr('href');
	var href_mac = jQuery('#edit-create-mac a').attr('href');

	link_param = '?company=' + company + '&region=' + region + '&province=' + province + '&store=' + store;

	jQuery('#edit-create-broadcast a').attr('href',href_broadcast+link_param);
	jQuery('#edit-create-mac a').attr('href',href_mac+link_param);

	var company_filter = '';
	var region_filter = '';
	var province_filter= '';
	var store_filter = '';


	jQuery('.form-item-company select').change(function() {
		company = jQuery(this).val();
		link_param = '?company=' + company;
		company_filter = href_broadcast + link_param;
		jQuery('#edit-create-broadcast a').attr('href',company_filter);
	});
	// Vi su dung ajax nen phai su dung DOM tren document, khi click vao select moi gan change event
	jQuery(document).on('click','#edit-header-filter .form-item-region select',function() {
		jQuery('#edit-header-filter .form-item-region select').change(function() {
			region = jQuery(this).val();
			link_param = '?company=' + jQuery('#edit-company').val();
			link_param += '&region=' + region;
			region_filter = href_broadcast + link_param;
			jQuery('#edit-create-broadcast a').attr('href',region_filter);
		});

	});

	// Vi su dung ajax nen phai su dung DOM tren document, khi click vao select moi gan change event
	jQuery(document).on('click','#edit-header-filter .form-item-province select',function() {
		jQuery('#edit-header-filter .form-item-province select').change(function() {
			province = jQuery(this).val();
			link_param = '?company=' + jQuery('#edit-company').val();
			link_param += '&region=' + jQuery('#edit-header-filter .form-item-region select').val();
			link_param += '&province=' + province;
			province_filter = href_broadcast + link_param;
			jQuery('#edit-create-broadcast a').attr('href',province_filter);
		});

	});

	jQuery(document).on('click','#edit-header-filter .form-item-store select',function() {
		jQuery('#edit-header-filter .form-item-store select').change(function() {
			store = jQuery(this).val();
			link_param = '?company=' + jQuery('#edit-company').val();
			link_param += '&region=' + jQuery('#edit-header-filter .form-item-region select').val();
			link_param += '&province=' + jQuery('#edit-province').val();
			link_param += '&store=' + store;
			store_filter = href_broadcast + link_param;
			jQuery('#edit-create-broadcast a').attr('href',store_filter);
		});

	});


	// Phan MAC khi cac trường filter được chọn
	var link_create_mac = '';

	jQuery('.form-item-company select').change(function() {
		company = jQuery(this).val();
		link_create_mac = '?company=' + company;
		company_filter = href_mac + link_create_mac;
		jQuery('#edit-create-mac a').attr('href',company_filter);
	});
	// Vi su dung ajax nen phai su dung DOM tren document, khi click vao select moi gan change event
	jQuery(document).on('click','#edit-header-filter .form-item-region select',function() {
		jQuery('#edit-header-filter .form-item-region select').change(function() {
			region = jQuery(this).val();
			link_create_mac = '?company=' + jQuery('#edit-company').val();
			link_create_mac += '&region=' + region;
			region_filter = href_mac + link_create_mac;
			jQuery('#edit-create-mac a').attr('href',region_filter);
		});

	});

	// Vi su dung ajax nen phai su dung DOM tren document, khi click vao select moi gan change event
	jQuery(document).on('click','#edit-header-filter .form-item-province select',function() {
		jQuery('#edit-header-filter .form-item-province select').change(function() {
			province = jQuery(this).val();
			link_create_mac = '?company=' + jQuery('#edit-company').val();
			link_create_mac += '&region=' + jQuery('#edit-header-filter .form-item-region select').val();
			link_create_mac += '&province=' + province;
			var province_filter = href_mac + link_create_mac;
			jQuery('#edit-create-mac a').attr('href',province_filter);
		});

	});

	jQuery(document).on('click','#edit-header-filter .form-item-store select',function() {
		jQuery('#edit-header-filter .form-item-store select').change(function() {
			store = jQuery(this).val();
			link_create_mac = '?company=' + jQuery('#edit-company').val();
			link_create_mac += '&region=' + jQuery('#edit-header-filter .form-item-region select').val();
			link_create_mac += '&province=' + jQuery('#edit-province').val();
			link_create_mac += '&store=' + store;
			store_filter = href_mac + link_create_mac;
			jQuery('#edit-create-mac a').attr('href',store_filter);
		});
	});


	// Get video duration
    var myVideos = [];
	window.URL = window.URL || window.webkitURL;
	function setFileInfo(files) {
	  myVideos.push(files[0]);
	  var video = document.createElement('video');
	  video.preload = 'metadata';
	  video.onloadedmetadata = function() {
	    window.URL.revokeObjectURL(this.src)
	    var duration = video.duration;
	    myVideos[myVideos.length-1].duration = duration;
	    updateInfos();
	  }
	  video.src = URL.createObjectURL(files[0]);;
	}

	function updateInfos(){
	      // document.querySelector('#infos').innerHTML="";
	  	for(i=0;i<myVideos.length;i++){
	      var upload_duration = myVideos[i].duration;
	      upload_duration = Math.floor(upload_duration);
	      document.querySelector("input[name='file_duration']").value = upload_duration;
	      
	    }  
	}

	// Trường hợp mới upload file lần đầu
	if(document.querySelector("input[name='files[push_myfile]']")) {
		document.querySelector("input[name='files[push_myfile]']").addEventListener('change',function() {
			setFileInfo(this.files);
			if(this.files[0].type == 'image/jpeg' || this.files[0].type == 'image/png' || this.files[0].type == 'image/gif') {
				document.querySelector("#file_duration").setAttribute('class','col-md-6');
				document.querySelector("#file_duration label").innerHTML = 'Thời gian trình chiếu ảnh';
			} else {
				document.querySelector("#file_duration").setAttribute('class','col-md-6 file_duration_hidden');
				document.querySelector("#file_duration label").innerHTML = 'Thời gian';
			}

		});
	}

	// Truong hop cac file khong duoc cap nhat thoi lương ngay tu dau
	if(document.querySelector("input[name='file_duration']")){
		if(!document.querySelector("input[name='file_duration']").value) {
			if(document.querySelector("#preview_mymedia video")) {
				var myVideoPlayer = document.querySelector("#preview_mymedia video");
				myVideoPlayer.addEventListener('loadedmetadata', function () {
				    var duration = myVideoPlayer.duration;
				    duration = Math.floor(duration);
				    console.log(duration);
				    document.querySelector("input[name='file_duration']").value = duration;
				});
			}
		} 
	}
	//Trường hợp khi bắt đầu vào trang sửa
	if(document.querySelector("#edit-push-myfile-upload img[title='image/jpeg']") || document.querySelector("#edit-push-myfile-upload img[title='image/png']") || document.querySelector("#edit-push-myfile-upload img[title='image/gif']")) {
		document.querySelector("#file_duration").setAttribute('class','col-md-6');
		document.querySelector("#file_duration label").innerHTML = 'Thời gian trình chiếu ảnh';
	}
	
	// Trường hợp vào sửa video có thể là ảnh có thể là video
	if(document.querySelector("input[name='push_myfile_remove_button']")) {
		jQuery(document).on('change',"input[name='files[push_myfile]']",function() {
			setFileInfo(this.files);
			if(this.files[0].type == 'image/jpeg' || this.files[0].type == 'image/png' || this.files[0].type == 'image/gif') {
				document.querySelector("#file_duration").setAttribute('class','col-md-6');
				document.querySelector("#file_duration label").innerHTML = 'Thời gian trình chiếu ảnh';
			} else {
				document.querySelector("#file_duration").setAttribute('class','col-md-6 file_duration_hidden');
				document.querySelector("#file_duration label").innerHTML = 'Thời gian';
			}
		})
	}


});