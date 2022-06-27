/*
 Created:	1-Nov-2021 14:27:23
 Author:	HS5TQA/Atten
*/

#include "webservice.h"
#include "m17.h"
#include "I2S.h"

extern hw_timer_t *timer;
extern char current_module;

// Web Server;
WebServer server(80);

String webString;

bool defaultSetting = false;

void serviceHandle()
{
	server.handleClient();
}
void setHTML(byte page)
{
	webString = "<html><head>\n";
	webString += "<meta charset=\"utf-8\">";
	webString += "<style>\nhdr1{background-color: powderblue;color: white;vertical-align: middle;text-align: center;font-size: 16px;font-weight: bold;}\n</style>\n";
	if (page == 0)
		webString += "<meta http-equiv=\"refresh\" content=\"10;url=http://" + WiFi.localIP().toString() + "\"> \n";
	webString += "<meta http - equiv = \"content-type\" content = \"text/html; charset=utf-8\" / > \n";

	webString += "<style>\n"
				 ".topnav{"
				 "position:relative;"
				 "z-index:2;"
				 "font-size:25px;"
				 "background-color:#5f5f5f;"
				 "color:#f1f1f1;"
				 "width:100%;"
				 "padding:10px;"
				 "letter-spacing:3px;"
				 "box-shadow:0 10px 10px 0 rgba(0,0,0,0.16);"
				 "font-family:\"Segoe UI\",Tahoma,sans-serif;"
				 "}\n"
				 ".title_hdr{"
				 "text-align: center;"
				 //"width: auto;"
				 "margin: 0 auto;"
				 "background: darkblue;"
				 "color: white;"
				 "font-size: 10px;"
				 "border-radius: 5px;"
				 "}\n"
				 ".title_value{"
				 "width: auto;"
				 "margin: 0 auto;"
				 "text-align: center;"
				 "font-size: 14px;"
				 "color: darkgreen;"
				 "background: white;"
				 "border-radius: 5px;"
				 "}\n"
				 ".L1{"
				 "text-align: center;"
				 "width: 33%;"
				 "margin: 1px;"
				 "background: red;"
				 "color: sandybrown;"
				 "font-size: 10px;"
				 "border-radius: 5px;"
				 "font-weight: bold;"
				 "}\n"
				 ".L2{"
				 "text-align: center;"
				 "width: 33%;"
				 "margin: 1px;"
				 "background: yellow;"
				 "color: black;"
				 "font-size: 10px;"
				 "border-radius: 5px;"
				 "font-weight: bold;"
				 "}\n"
				 ".L3{"
				 "text-align: center;"
				 "width: 33%;"
				 "margin: 1px;"
				 "background: blue;"
				 "color: lightgray;"
				 "font-size: 10px;"
				 "border-radius: 5px;"
				 "font-weight: bold;"
				 "}\n";
	webString += F(".col-pad{width: 500px;}");
	webString += F(".form-control{display:block;width:100%;height:34px;padding:6px 12px;font-size:14px;line-height:1.42857143;color:#555;background-color:#fff;background-image:none;border:1px solid #ccc;border-radius:4px;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075);box-shadow:inset 0 1px 1px rgba(0,0,0,.075);-webkit-transition:border-color ease-in-out .15s,box-shadow ease-in-out .15s;transition:border-color ease-in-out .15s,box-shadow ease-in-out .15s}");
	webString += F(".btn{display:inline-block;margin-bottom:0;font-weight:400;text-align:center;vertical-align:middle;cursor:pointer;background-image:none;border:1px solid transparent;white-space:nowrap;padding:6px 12px;font-size:14px;line-height:1.42857143;border-radius:4px;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}.btn:focus,.btn:active:focus,.btn.active:focus{outline:thin dotted;outline:5px auto -webkit-focus-ring-color;outline-offset:-2px}.btn:hover,.btn:focus{color:#333;text-decoration:none}.btn:active,.btn.active{outline:0;background-image:none;-webkit-box-shadow:inset 0 3px 5px rgba(0,0,0,.125);box-shadow:inset 0 3px 5px rgba(0,0,0,.125)}.btn.disabled,.btn[disabled],fieldset[disabled] .btn{cursor:not-allowed;pointer-events:none;opacity:.65;filter:alpha(opacity=65);-webkit-box-shadow:none;box-shadow:none}.btn-default{color:#333;background-color:#fff;border-color:#ccc}.btn-default:hover,.btn-default:focus,.btn-default:active,.btn-default.active,.open .dropdown-toggle.btn-default{color:#333;background-color:#ebebeb;border-color:#adadad}.btn-default:active,.btn-default.active,.open .dropdown-toggle.btn-default{background-image:none}.btn-default.disabled,.btn-default[disabled],fieldset[disabled] .btn-default,.btn-default.disabled:hover,.btn-default[disabled]:hover,fieldset[disabled] .btn-default:hover,.btn-default.disabled:focus,.btn-default[disabled]:focus,fieldset[disabled] .btn-default:focus,.btn-default.disabled:active,.btn-default[disabled]:active,fieldset[disabled] .btn-default:active,.btn-default.disabled.active,.btn-default[disabled].active,fieldset[disabled] .btn-default.active{background-color:#fff;border-color:#ccc}.btn-default .badge{color:#fff;background-color:#333}");
	webString += F(".btn-danger {color: #fff;background-color: #d9534f;border-color: #d43f3a;}");
	webString += F(".btn-primary {color: #fff;background-color: #428bca;border-color: #357ebd;}");
	webString += F(".clearfix:after, .container:after, .container-fluid:after, .row:after, .form-horizontal .form-group:after, .btn-toolbar:after, .btn-group-vertical>.btn-group:after, .nav:after, .navbar:after, .navbar-header:after, .navbar-collapse:after, .pager:after, .panel-body:after, .modal-footer:after {clear: both;}");
	webString += F(".clearfix:before, .clearfix:after, .container:before, .container:after, .container-fluid:before, .container-fluid:after, .row:before, .row:after, .form-horizontal .form-group:before, .form-horizontal .form-group:after, .btn-toolbar:before, .btn-toolbar:after, .btn-group-vertical>.btn-group:before, .btn-group-vertical>.btn-group:after, .nav:before, .nav:after, .navbar:before, .navbar:after, .navbar-header:before, .navbar-header:after, .navbar-collapse:before, .navbar-collapse:after, .pager:before, .pager:after, .panel-body:before, .panel-body:after, .modal-footer:before, .modal-footer:after {content: \" \";display: table;}");
	webString += F(".nav{margin-bottom:0;padding-left:0;list-style:none}.nav>li{position:relative;display:block}.nav>li>a{position:relative;display:block;padding:10px 15px}.nav>li>a:hover,.nav>li>a:focus{text-decoration:none;background-color:#eee}.nav>li.disabled>a{color:#999}.nav>li.disabled>a:hover,.nav>li.disabled>a:focus{color:#999;text-decoration:none;background-color:transparent;cursor:not-allowed}.nav .open>a,.nav .open>a:hover,.nav .open>a:focus{background-color:#eee;border-color:#428bca}.nav .nav-divider{height:1px;margin:9px 0;overflow:hidden;background-color:#e5e5e5}.nav>li>a>img{max-width:none}.nav-tabs{border-bottom:1px solid #ddd}.nav-tabs>li{float:left;margin-bottom:-1px}.nav-tabs>li>a{margin-right:0px;line-height:1.42857143;border:1px solid #ddd;border-radius:10px 10px 0 0}.nav-tabs>li>a:hover{border-color:#eee #eee #ddd}.nav-tabs>li.active>a,.nav-tabs>li.active>a:hover,.nav-tabs>li.active>a:focus{color:#428bca;background-color:#e5e5e5;border:1px solid #ddd;border-bottom-color:transparent;cursor:default}.nav-tabs.nav-justified{width:100%;border-bottom:0}.nav-tabs.nav-justified>li{float:none}.nav-tabs.nav-justified>li>a{text-align:center;margin-bottom:5px}.nav-tabs.nav-justified>.dropdown .dropdown-menu{top:auto;left:auto}@media (min-width:768px){.nav-tabs.nav-justified>li{display:table-cell;width:1%}.nav-tabs.nav-justified>li>a{margin-bottom:0}}.nav-tabs.nav-justified>li>a{margin-right:0;border-radius:4px}.nav-tabs.nav-justified>.active>a,.nav-tabs.nav-justified>.active>a:hover,.nav-tabs.nav-justified>.active>a:focus{border:1px solid #ddd}@media (min-width:768px){.nav-tabs.nav-justified>li>a{border-bottom:1px solid #ddd;border-radius:4px 4px 0 0}.nav-tabs.nav-justified>.active>a,.nav-tabs.nav-justified>.active>a:hover,.nav-tabs.nav-justified>.active>a:focus{border-bottom-color:#fff}}.nav-pills>li{float:left}.nav-pills>li>a{border-radius:4px}.nav-pills>li+li{margin-left:2px}.nav-pills>li.active>a,.nav-pills>li.active>a:hover,.nav-pills>li.active>a:focus{color:#fff;background-color:#428bca}.nav-stacked>li{float:none}.nav-stacked>li+li{margin-top:2px;margin-left:0}.nav-justified{width:100%}.nav-justified>li{float:none}.nav-justified>li>a{text-align:center;margin-bottom:5px}.nav-justified>.dropdown .dropdown-menu{top:auto;left:auto}@media (min-width:768px){.nav-justified>li{display:table-cell;width:1%}.nav-justified>li>a{margin-bottom:0}}.nav-tabs-justified{border-bottom:0}.nav-tabs-justified>li>a{margin-right:0;border-radius:4px}.nav-tabs-justified>.active>a,.nav-tabs-justified>.active>a:hover,.nav-tabs-justified>.active>a:focus{border:1px solid #ddd}@media (min-width:768px){.nav-tabs-justified>li>a{border-bottom:1px solid #ddd;border-radius:4px 4px 0 0}.nav-tabs-justified>.active>a,.nav-tabs-justified>.active>a:hover,.nav-tabs-justified>.active>a:focus{border-bottom-color:#fff}}.tab-content>.tab-pane{display:none}.tab-content>.active{display:block}.nav-tabs .dropdown-menu{margin-top:-1px;border-top-right-radius:0;border-top-left-radius:0}");
	webString += F(".form-group{margin-bottom:15px}.radio,.checkbox{display:block;min-height:20px;margin-top:10px;margin-bottom:10px;padding-left:20px}.radio label,.checkbox label{display:inline;font-weight:400;cursor:pointer}.radio input[type=radio],.radio-inline input[type=radio],.checkbox input[type=checkbox],.checkbox-inline input[type=checkbox]{float:left;margin-left:-20px}.radio+.radio,.checkbox+.checkbox{margin-top:-5px}.radio-inline,.checkbox-inline{display:inline-block;padding-left:20px;margin-bottom:0;vertical-align:middle;font-weight:400;cursor:pointer}.radio-inline+.radio-inline,.checkbox-inline+.checkbox-inline{margin-top:0;margin-left:10px}input[type=radio][disabled],input[type=checkbox][disabled],.radio[disabled],.radio-inline[disabled],.checkbox[disabled],.checkbox-inline[disabled],fieldset[disabled] input[type=radio],fieldset[disabled] input[type=checkbox],fieldset[disabled] .radio,fieldset[disabled] .radio-inline,fieldset[disabled] .checkbox,fieldset[disabled] .checkbox-inline{cursor:not-allowed}.input-sm{height:30px;padding:5px 10px;font-size:12px;line-height:1.5;border-radius:3px}select.input-sm{height:30px;line-height:30px}textarea.input-sm,select[multiple].input-sm{height:auto}.input-lg{height:46px;padding:10px 16px;font-size:18px;line-height:1.33;border-radius:6px}select.input-lg{height:46px;line-height:46px}textarea.input-lg,select[multiple].input-lg{height:auto}.has-feedback{position:relative}.has-feedback .form-control{padding-right:42.5px}.has-feedback .form-control-feedback{position:absolute;top:25px;right:0;display:block;width:34px;height:34px;line-height:34px;text-align:center}.has-success .help-block,.has-success .control-label,.has-success .radio,.has-success .checkbox,.has-success .radio-inline,.has-success .checkbox-inline{color:#3c763d}.has-success .form-control{border-color:#3c763d;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075);box-shadow:inset 0 1px 1px rgba(0,0,0,.075)}.has-success .form-control:focus{border-color:#2b542c;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075),0 0 6px #67b168;box-shadow:inset 0 1px 1px rgba(0,0,0,.075),0 0 6px #67b168}.has-success .input-group-addon{color:#3c763d;border-color:#3c763d;background-color:#dff0d8}.has-success .form-control-feedback{color:#3c763d}.has-warning .help-block,.has-warning .control-label,.has-warning .radio,.has-warning .checkbox,.has-warning .radio-inline,.has-warning .checkbox-inline{color:#8a6d3b}.has-warning .form-control{border-color:#8a6d3b;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075);box-shadow:inset 0 1px 1px rgba(0,0,0,.075)}.has-warning .form-control:focus{border-color:#66512c;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075),0 0 6px #c0a16b;box-shadow:inset 0 1px 1px rgba(0,0,0,.075),0 0 6px #c0a16b}.has-warning .input-group-addon{color:#8a6d3b;border-color:#8a6d3b;background-color:#fcf8e3}.has-warning .form-control-feedback{color:#8a6d3b}.has-error .help-block,.has-error .control-label,.has-error .radio,.has-error .checkbox,.has-error .radio-inline,.has-error .checkbox-inline{color:#a94442}.has-error .form-control{border-color:#a94442;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075);box-shadow:inset 0 1px 1px rgba(0,0,0,.075)}.has-error .form-control:focus{border-color:#843534;-webkit-box-shadow:inset 0 1px 1px rgba(0,0,0,.075),0 0 6px #ce8483;box-shadow:inset 0 1px 1px rgba(0,0,0,.075),0 0 6px #ce8483}.has-error .input-group-addon{color:#a94442;border-color:#a94442;background-color:#f2dede}.has-error .form-control-feedback{color:#a94442}.form-control-static{margin-bottom:0}.help-block{display:block;margin-top:5px;margin-bottom:10px;color:#737373}@media (min-width:768px){.form-inline .form-group{display:inline-block;margin-bottom:0;vertical-align:middle}.form-inline .form-control{display:inline-block;width:auto;vertical-align:middle}.form-inline .input-group>.form-control{width:100%}.form-inline .control-label{margin-bottom:0;vertical-align:middle}.form-inline .radio,.form-inline .checkbox{display:inline-block;margin-top:0;margin-bottom:0;padding-left:0;vertical-align:middle}.form-inline .radio input[type=radio],.form-inline .checkbox input[type=checkbox]{float:none;margin-left:0}.form-inline .has-feedback .form-control-feedback{top:0}}.form-horizontal .control-label,.form-horizontal .radio,.form-horizontal .checkbox,.form-horizontal .radio-inline,.form-horizontal .checkbox-inline{margin-top:0;margin-bottom:0;padding-top:0px}.form-horizontal .radio,.form-horizontal .checkbox{min-height:27px}.form-horizontal .form-group{margin-left:-15px;margin-right:-15px}.form-horizontal .form-control-static{padding-top:7px}@media (min-width:768px){.form-horizontal .control-label{text-align:right}}.form-horizontal .has-feedback .form-control-feedback{top:0;right:15px}");
	webString += F(".col-xs-1,.col-sm-1,.col-md-1,.col-lg-1,.col-xs-2,.col-sm-2,.col-md-2,.col-lg-2,.col-xs-3,.col-sm-3,.col-md-3,.col-lg-3,.col-xs-4,.col-sm-4,.col-md-4,.col-lg-4,.col-xs-5,.col-sm-5,.col-md-5,.col-lg-5,.col-xs-6,.col-sm-6,.col-md-6,.col-lg-6,.col-xs-7,.col-sm-7,.col-md-7,.col-lg-7,.col-xs-8,.col-sm-8,.col-md-8,.col-lg-8,.col-xs-9,.col-sm-9,.col-md-9,.col-lg-9,.col-xs-10,.col-sm-10,.col-md-10,.col-lg-10,.col-xs-11,.col-sm-11,.col-md-11,.col-lg-11,.col-xs-12,.col-sm-12,.col-md-12,.col-lg-12{position:relative;min-height:1px;padding-left:15px;padding-right:15px}.col-xs-1,.col-xs-2,.col-xs-3,.col-xs-4,.col-xs-5,.col-xs-6,.col-xs-7,.col-xs-8,.col-xs-9,.col-xs-10,.col-xs-11,.col-xs-12{float:left}.col-xs-12{width:100%}.col-xs-11{width:91.66666667%}.col-xs-10{width:83.33333333%}.col-xs-9{width:75%}.col-xs-8{width:66.66666667%}.col-xs-7{width:58.33333333%}.col-xs-6{width:50%}.col-xs-5{width:41.66666667%}.col-xs-4{width:33.33333333%}.col-xs-3{width:25%}.col-xs-2{width:16.66666667%}.col-xs-1{width:8.33333333%}.col-xs-pull-12{right:100%}.col-xs-pull-11{right:91.66666667%}.col-xs-pull-10{right:83.33333333%}.col-xs-pull-9{right:75%}.col-xs-pull-8{right:66.66666667%}.col-xs-pull-7{right:58.33333333%}.col-xs-pull-6{right:50%}.col-xs-pull-5{right:41.66666667%}.col-xs-pull-4{right:33.33333333%}.col-xs-pull-3{right:25%}.col-xs-pull-2{right:16.66666667%}.col-xs-pull-1{right:8.33333333%}.col-xs-pull-0{right:0}.col-xs-push-12{left:100%}.col-xs-push-11{left:91.66666667%}.col-xs-push-10{left:83.33333333%}.col-xs-push-9{left:75%}.col-xs-push-8{left:66.66666667%}.col-xs-push-7{left:58.33333333%}.col-xs-push-6{left:50%}.col-xs-push-5{left:41.66666667%}.col-xs-push-4{left:33.33333333%}.col-xs-push-3{left:25%}.col-xs-push-2{left:16.66666667%}.col-xs-push-1{left:8.33333333%}.col-xs-push-0{left:0}.col-xs-offset-12{margin-left:100%}.col-xs-offset-11{margin-left:91.66666667%}.col-xs-offset-10{margin-left:83.33333333%}.col-xs-offset-9{margin-left:75%}.col-xs-offset-8{margin-left:66.66666667%}.col-xs-offset-7{margin-left:58.33333333%}.col-xs-offset-6{margin-left:50%}.col-xs-offset-5{margin-left:41.66666667%}.col-xs-offset-4{margin-left:33.33333333%}.col-xs-offset-3{margin-left:25%}.col-xs-offset-2{margin-left:16.66666667%}.col-xs-offset-1{margin-left:8.33333333%}.col-xs-offset-0{margin-left:0}@media (min-width:768px){.col-sm-1,.col-sm-2,.col-sm-3,.col-sm-4,.col-sm-5,.col-sm-6,.col-sm-7,.col-sm-8,.col-sm-9,.col-sm-10,.col-sm-11,.col-sm-12{float:left}.col-sm-12{width:100%}.col-sm-11{width:91.66666667%}.col-sm-10{width:83.33333333%}.col-sm-9{width:75%}.col-sm-8{width:66.66666667%}.col-sm-7{width:58.33333333%}.col-sm-6{width:50%}.col-sm-5{width:41.66666667%}.col-sm-4{width:33.33333333%}.col-sm-3{width:25%}.col-sm-2{width:16.66666667%}.col-sm-1{width:8.33333333%}.col-sm-pull-12{right:100%}.col-sm-pull-11{right:91.66666667%}.col-sm-pull-10{right:83.33333333%}.col-sm-pull-9{right:75%}.col-sm-pull-8{right:66.66666667%}.col-sm-pull-7{right:58.33333333%}.col-sm-pull-6{right:50%}.col-sm-pull-5{right:41.66666667%}.col-sm-pull-4{right:33.33333333%}.col-sm-pull-3{right:25%}.col-sm-pull-2{right:16.66666667%}.col-sm-pull-1{right:8.33333333%}.col-sm-pull-0{right:0}.col-sm-push-12{left:100%}.col-sm-push-11{left:91.66666667%}.col-sm-push-10{left:83.33333333%}.col-sm-push-9{left:75%}.col-sm-push-8{left:66.66666667%}.col-sm-push-7{left:58.33333333%}.col-sm-push-6{left:50%}.col-sm-push-5{left:41.66666667%}.col-sm-push-4{left:33.33333333%}.col-sm-push-3{left:25%}.col-sm-push-2{left:16.66666667%}.col-sm-push-1{left:8.33333333%}.col-sm-push-0{left:0}.col-sm-offset-12{margin-left:100%}.col-sm-offset-11{margin-left:91.66666667%}.col-sm-offset-10{margin-left:83.33333333%}.col-sm-offset-9{margin-left:75%}.col-sm-offset-8{margin-left:66.66666667%}.col-sm-offset-7{margin-left:58.33333333%}.col-sm-offset-6{margin-left:50%}.col-sm-offset-5{margin-left:41.66666667%}.col-sm-offset-4{margin-left:33.33333333%}.col-sm-offset-3{margin-left:25%}.col-sm-offset-2{margin-left:16.66666667%}.col-sm-offset-1{margin-left:8.33333333%}.col-sm-offset-0{margin-left:0}}@media (min-width:992px){.col-md-1,.col-md-2,.col-md-3,.col-md-4,.col-md-5,.col-md-6,.col-md-7,.col-md-8,.col-md-9,.col-md-10,.col-md-11,.col-md-12{float:left}.col-md-12{width:100%}.col-md-11{width:91.66666667%}.col-md-10{width:83.33333333%}.col-md-9{width:75%}.col-md-8{width:66.66666667%}.col-md-7{width:58.33333333%}.col-md-6{width:50%}.col-md-5{width:41.66666667%}.col-md-4{width:33.33333333%}.col-md-3{width:25%}.col-md-2{width:16.66666667%}.col-md-1{width:8.33333333%}.col-md-pull-12{right:100%}.col-md-pull-11{right:91.66666667%}.col-md-pull-10{right:83.33333333%}.col-md-pull-9{right:75%}.col-md-pull-8{right:66.66666667%}.col-md-pull-7{right:58.33333333%}.col-md-pull-6{right:50%}.col-md-pull-5{right:41.66666667%}.col-md-pull-4{right:33.33333333%}.col-md-pull-3{right:25%}.col-md-pull-2{right:16.66666667%}.col-md-pull-1{right:8.33333333%}.col-md-pull-0{right:0}.col-md-push-12{left:100%}.col-md-push-11{left:91.66666667%}.col-md-push-10{left:83.33333333%}.col-md-push-9{left:75%}.col-md-push-8{left:66.66666667%}.col-md-push-7{left:58.33333333%}.col-md-push-6{left:50%}.col-md-push-5{left:41.66666667%}.col-md-push-4{left:33.33333333%}.col-md-push-3{left:25%}.col-md-push-2{left:16.66666667%}.col-md-push-1{left:8.33333333%}.col-md-push-0{left:0}.col-md-offset-12{margin-left:100%}.col-md-offset-11{margin-left:91.66666667%}.col-md-offset-10{margin-left:83.33333333%}.col-md-offset-9{margin-left:75%}.col-md-offset-8{margin-left:66.66666667%}.col-md-offset-7{margin-left:58.33333333%}.col-md-offset-6{margin-left:50%}.col-md-offset-5{margin-left:41.66666667%}.col-md-offset-4{margin-left:33.33333333%}.col-md-offset-3{margin-left:25%}.col-md-offset-2{margin-left:16.66666667%}.col-md-offset-1{margin-left:8.33333333%}.col-md-offset-0{margin-left:0}}@media (min-width:1200px){.col-lg-1,.col-lg-2,.col-lg-3,.col-lg-4,.col-lg-5,.col-lg-6,.col-lg-7,.col-lg-8,.col-lg-9,.col-lg-10,.col-lg-11,.col-lg-12{float:left}.col-lg-12{width:100%}.col-lg-11{width:91.66666667%}.col-lg-10{width:83.33333333%}.col-lg-9{width:75%}.col-lg-8{width:66.66666667%}.col-lg-7{width:58.33333333%}.col-lg-6{width:50%}.col-lg-5{width:41.66666667%}.col-lg-4{width:33.33333333%}.col-lg-3{width:25%}.col-lg-2{width:16.66666667%}.col-lg-1{width:8.33333333%}.col-lg-pull-12{right:100%}.col-lg-pull-11{right:91.66666667%}.col-lg-pull-10{right:83.33333333%}.col-lg-pull-9{right:75%}.col-lg-pull-8{right:66.66666667%}.col-lg-pull-7{right:58.33333333%}.col-lg-pull-6{right:50%}.col-lg-pull-5{right:41.66666667%}.col-lg-pull-4{right:33.33333333%}.col-lg-pull-3{right:25%}.col-lg-pull-2{right:16.66666667%}.col-lg-pull-1{right:8.33333333%}.col-lg-pull-0{right:0}.col-lg-push-12{left:100%}.col-lg-push-11{left:91.66666667%}.col-lg-push-10{left:83.33333333%}.col-lg-push-9{left:75%}.col-lg-push-8{left:66.66666667%}.col-lg-push-7{left:58.33333333%}.col-lg-push-6{left:50%}.col-lg-push-5{left:41.66666667%}.col-lg-push-4{left:33.33333333%}.col-lg-push-3{left:25%}.col-lg-push-2{left:16.66666667%}.col-lg-push-1{left:8.33333333%}.col-lg-push-0{left:0}.col-lg-offset-12{margin-left:100%}.col-lg-offset-11{margin-left:91.66666667%}.col-lg-offset-10{margin-left:83.33333333%}.col-lg-offset-9{margin-left:75%}.col-lg-offset-8{margin-left:66.66666667%}.col-lg-offset-7{margin-left:58.33333333%}.col-lg-offset-6{margin-left:50%}.col-lg-offset-5{margin-left:41.66666667%}.col-lg-offset-4{margin-left:33.33333333%}.col-lg-offset-3{margin-left:25%}.col-lg-offset-2{margin-left:16.66666667%}.col-lg-offset-1{margin-left:8.33333333%}.col-lg-offset-0{margin-left:0}}");
	// webString += "<link rel=\"stylesheet\" href=\"//maxcdn.bootstrapcdn.com/bootstrap/3.1.1/css/bootstrap.min.css\" />\n";
	webString += "</style>\n";

	if (page == 0)
	{
		///////////// google charts script
		webString += "<script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script> \n";
		webString += "   <script type=\"text/javascript\"> \n";
		webString += "    google.charts.load('current', {'packages':['corechart','gauge']}); \n";

		webString += "    google.charts.setOnLoadCallback(drawBatGauge); \n";
		webString += "function drawBatGauge() { \n";
		webString += "      var data = google.visualization.arrayToDataTable([ \n";
		webString += "        ['Label', 'Value'], ";
		webString += "        ['dBm',  ";
		webString += String(WiFi.RSSI());
		webString += " ], ";
		webString += "       ]); \n";
		// setup the google chart options here
		webString += "    var options = {";
		webString += "width: 250, height: 150,";
		webString += "min: -100, max: -30,";
		webString += "redFrom: -100, redTo: -90,";
		webString += "greenFrom: -90, greenTo: -60,";
		webString += "yellowFrom: -60, yellowTo: -30,";
		webString += "minorTicks: 5";
		webString += "    }; \n";
		webString += "   var chart = new google.visualization.Gauge(document.getElementById('chart_divGaugeBat')); \n";
		webString += "  chart.draw(data, options); \n";
		webString += "}\n\n";

		////////////
		webString += "    </script> \n";
	}
	String strActiveP1 = "";
	String strActiveP2 = "";
	String strActiveP3 = "";
	String strActiveP4 = "";
	String strActiveP5 = "";
	String strActiveP6 = "";
	String strActiveP7 = "";
	String strActiveP8 = "";
	if (page == 7)
		strActiveP8 = "class=active";
	else if (page == 6)
		strActiveP7 = "class=active";
	else if (page == 5)
		strActiveP6 = "class=active";
	else if (page == 4)
		strActiveP5 = "class=active";
	else if (page == 3)
		strActiveP4 = "class=active";
	else if (page == 2)
		strActiveP3 = "class=active";
	else if (page == 1)
		strActiveP2 = "class=active";
	else if (page == 0)
		strActiveP1 = "class=active";
	webString += "</head><body>\n";
	webString += "<div class='w3-card-2 topnav notranslate' id='topnav'><b>M17 Analog Gateway</div>\n";
	webString += "<div class=\"row\">\n";
	webString += "<ul class=\"nav nav-tabs\" style=\"margin: 25px;\">\n";
	webString += "<li role=\"presentation\"" + strActiveP1 + ">\n<a href=\"/\" id=\"channel_link_private_view\">Dash Board</a>\n</li>\n";
#ifdef SDCARD
	webString += "<li role=\"presentation\"" + strActiveP2 + ">\n<a href=\"/data\" id=\"channel_link_api_keys\">Storage</a>\n</li>\n";
#endif
	webString += "<li role=\"presentation\"" + strActiveP3 + ">\n<a href=\"/config\" id=\"channel_link_settings\">Setting</a>\n</li>\n";
#ifdef SA818
	webString += "<li role=\"presentation\"" + strActiveP7 + ">\n<a href=\"/radio\" id=\"channel_link_radio\">Radio</a>\n</li>\n";
#endif
	webString += "<li role=\"presentation\"" + strActiveP8 + ">\n<a href=\"/vpn\" id=\"channel_link_radio\">VPN</a>\n</li>\n";
	webString += "<li role=\"presentation\"" + strActiveP4 + ">\n<a href=\"/service\" id=\"channel_link_service\">Service</a>\n</li>\n";
	webString += "<li role=\"presentation\"" + strActiveP5 + ">\n<a href=\"/system\" id=\"channel_link_system\">System</a>\n</li>\n";
	webString += "<li role=\"presentation\"" + strActiveP6 + ">\n<a href=\"/firmware\" id=\"channel_link_firmware\">Firmware</a>\n</li>\n";
	webString += "</ul>\n</div>";

	if (page == 0)
	{
		char strTime[30];
		struct tm tmstruct;
		tmstruct.tm_year = 0;
		time_t tn = now() - systemUptime;
		getLocalTime(&tmstruct, 5000);
		sprintf(strTime, "%d-%02d-%02d %02d:%02d:%02d", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);

		webString += "<table style=\"width:800px;\"><tr><td>";
		webString += "<div style=\"width:300px\"><b>Last Readings at " + String(strTime) + "</b></div>\n";

		webString += "<div>CPU Temp: " + String((temprature_sens_read() - 32) / 1.8, 1) + "C</div> \n";
		webString += "<div>Free Heap:" + String(ESP.getFreeHeap()) + " Byte</div> \n";
		String uptime = String(day(tn) - 1, DEC) + "D " + String(hour(tn), DEC) + ":" + String(minute(tn), DEC) + ":" + String(second(tn), DEC);
		webString += "<div>System Uptime: " + uptime + "</div> \n";

		webString += "</td></tr><tr><td>\n";

		webString += "<table border=\"1\"><tr align=\"center\"><td class=\"hdr1\">WiFi Signal</td></tr>\n";
		webString += "<tr align=\"center\"><td><div id=\"chart_divGaugeBat\" style=\"width: 250px;\"></div><br /><hr width=\"50%\"></td>\n";
		webString += "</table>\n";

		webString += "</td></tr><br /><tr align=\"left\"><td>\n";

		webString += "<table border=\"0\"><tr align=\"center\"><th style=\"background-color:#A0C0C0 ;width:30px\">#</th><th style=\"background-color:#F0A0F0 ;width:120px\">Callsign</th><th style=\"background-color:#F0F0A0 ;width:80px\">Last heard</th></tr>\n";

		for (int i = 0; i < PKGLISTSIZE; i++)
		{
			if ((int)pkgList[i].time > 0)
			{
				webString += "<tr align=\"center\">";
				webString += "<td class=\"hdr2\">" + String(i + 1) + "</td>";
				webString += "<td class=\"hdr2\">" + String(&pkgList[i].calsign[0]) + "</td>";
				time_t tm = pkgList[i].time;
				localtime_r(&pkgList[i].time, &tmstruct);
				String ltime = String(tmstruct.tm_hour, DEC) + ":" + String(tmstruct.tm_min, DEC) + ":" + String(tmstruct.tm_sec, DEC);
				// String ltime = String(hour(pkgList[i].time), DEC) + ":" + String(minute(pkgList[i].time), DEC) + ":" + String(second(pkgList[i].time), DEC);
				webString += "<td class=\"hdr2\">" + ltime + "</td>";
				webString += "</tr>\n";
			}
		}
		webString += "</table>\n";
		webString += "</td></tr></table><br/>\n";
	}
	else if (page == 1)
	{
		// webString += "PAGE 2</br>\n";
	}
	else if (page == 2)
	{
		// webString += "PAGE 3</br>\n";
	}
	else if (page == 3)
	{
		webString += "<script type=\"text/javascript\">\n";
		webString += "function showVoxValue(newValue){\n";
		webString += "document.getElementById(\"voxShow\").innerHTML=newValue;}\n";
		webString += "function showMicValue(newValue){\n";
		webString += "document.getElementById(\"micShow\").innerHTML=newValue;}\n";
		webString += "</script>\n";
	}
	// webString += "</body></html>\n";
}

////////////////////////////////////////////////////////////
// handler for web server request: http://IpAddress/      //
////////////////////////////////////////////////////////////

void handle_root()
{
	setHTML(0);
	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked
	delay(100);
	webString.clear();
}

#ifdef SDCARD
void handle_storage()
{
	String dirname = "/";
	char strTime[100];

	if (server.args() > 0)
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "SD_INIT")
			{
				// SD.end();
				// if (!SD.begin(SDCARD_CS, spiSD, SDSPEED)) {
				//	Serial.println("SD CARD Initialization failed!");
				//	//return;
				// }
			}
		}
	}

	setHTML(1);
	uint8_t cardType = SD.cardType();

	webString += "<b>SD CARD TYPE:</b> ";
	if (cardType == CARD_NONE)
	{
		webString += "NOT FOUND\n";
	}
	else
	{
		if (cardType == CARD_MMC)
		{
			webString += "MMC\n";
		}
		else if (cardType == CARD_SD)
		{
			webString += "SDSC\n";
		}
		else if (cardType == CARD_SDHC)
		{
			webString += "SDHC\n";
		}
		else
		{
			webString += "UNKNOWN\n";
		}
		unsigned long cardSize = SD.cardSize() / (1024 * 1024);
		unsigned long cardTotal = SD.totalBytes() / (1024 * 1024);
		unsigned long cardUsed = SD.usedBytes() / (1024 * 1024);

		webString += "</br>";
		webString += "<b>SD Card Size: </b>";
		webString += String((double)cardSize / 1000, 1) + "GB</br>\n";
		webString += "<b>Total space: </b>";
		webString += String((unsigned long)cardTotal) + "MB</br>\n";
		webString += "<b>Used space: </b>";
		webString += String((unsigned long)cardUsed) + "MB</br>\n";

		webString += "</br><br>Listing directory: </b>" + dirname + "</br>\n";

		File root = SD.open(dirname);
		if (!root)
		{
			webString += "Failed to open directory\n";
			// return;
		}
		if (!root.isDirectory())
		{
			webString += "Not a directory";
			// return;
		}

		File file = root.openNextFile();
		webString += "<table border=\"1\"><tr align=\"center\" bgcolor=\"#03DDFC\"><td><b>DIRECTORY</b></td><td width=\"150\"><b>FILE NAME</b></td><td width=\"100\"><b>SIZE(Byte)</b></td><td width=\"170\"><b>DATE TIME</b></td><td><b>DEL</b></td></tr>";
		while (file)
		{
			if (file.isDirectory())
			{
				// webString += "<tr><td>DIR : ");
				webString += "<tr><td>" + String(file.name()) + "</td>";
				time_t t = file.getLastWrite();
				struct tm *tmstruct = localtime(&t);
				sprintf(strTime, "<td></td><td></td><td align=\"right\">%d-%02d-%02d %02d:%02d:%02d</td>", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
				webString += String(strTime);
				// if (levels) {
				//	listDir(fs, file.name(), levels - 1);
				// }
				webString += "<td></td></tr>\n";
			}
			else
			{
				/*Serial.print("  FILE: ");
				Serial.print(file.name());*/
				String fName = String(file.name()).substring(1);
				webString += "<tr><td>/</td><td align=\"right\"><a href=\"/download?FILE=" + fName + "\" target=\"_blank\">" + fName + "</a></td>";
				// Serial.print("  SIZE: ");
				webString += "<td align=\"right\">" + String(file.size()) + "</td>";
				time_t t = file.getLastWrite();
				struct tm *tmstruct = localtime(&t);
				sprintf(strTime, "<td align=\"right\">%d-%02d-%02d %02d:%02d:%02d</td>", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
				webString += String(strTime);
				webString += "<td align=\"center\"><a href=\"/delete?FILE=" + fName + "\">X</a></td></tr>\n";
			}
			file = root.openNextFile();
		}
		webString += "</table>";
	}

	webString += "</br><div><a href=\"/data?SD_INIT=OK\">[SD INIT]</a></div> \n";
	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked

	delay(100);
}

#endif

void handle_setting()
{
	bool wifiSTA = false;
	bool wifiAP = false;
	bool oledEN = false;
	bool aprsEn = false;

	if (defaultSetting)
	{
		disconnect_from_host(); // Restart M17 to Reflector
		defaultConfig();
	}
	else
	{
		if (server.args() > 0)
		{
			// #ifndef I2S_INTERNAL
			// 			bool adcIsr = timerAlarmEnabled(timer);
			// 			if (adcIsr)
			// 				timerAlarmDisable(timer);
			// #endif
			disconnect_from_host(); // Restart M17 to Reflector
			for (uint8_t i = 0; i < server.args(); i++)
			{
				// Serial.print("SERVER ARGS ");
				// Serial.print(server.argName(i));
				// Serial.print("=");
				// Serial.println(server.arg(i));
				if (server.argName(i) == "aprsEnable")
				{
					if (server.arg(i) != "")
					{
						if (String(server.arg(i)) == "OK")
						{
							aprsEn = true;
						}
					}
				}

				if (server.argName(i) == "oledEnable")
				{
					if (server.arg(i) != "")
					{
						if (String(server.arg(i)) == "OK")
						{
							oledEN = true;
						}
					}
				}

				if (server.argName(i) == "wifiAP")
				{
					if (server.arg(i) != "")
					{
						if (String(server.arg(i)) == "OK")
						{
							wifiAP = true;
							// if ((config.wifi_mode == WIFI_STA_FIX) || (config.wifi_mode == WIFI_AP_STA_FIX))
							//	config.wifi_mode = WIFI_AP_STA_FIX;
							// else
							//	config.wifi_mode = WIFI_AP_FIX;
						}
					}
				}
				if (server.argName(i) == "wificlient")
				{
					if (server.arg(i) != "")
					{
						if (String(server.arg(i)) == "OK")
						{
							wifiSTA = true;
							// if ((config.wifi_mode == WIFI_AP_FIX)||(config.wifi_mode == WIFI_AP_STA_FIX))
							//	config.wifi_mode = WIFI_AP_STA_FIX;
							// else
							//	config.wifi_mode = WIFI_STA_FIX;
						}
					}
				}

				if (server.argName(i) == "gpsLat")
				{
					if (server.arg(i) != "")
					{
						config.gps_lat = server.arg(i).toFloat();
					}
				}
				if (server.argName(i) == "gpsLon")
				{
					if (server.arg(i) != "")
					{
						config.gps_lon = server.arg(i).toFloat();
					}
				}
				if (server.argName(i) == "gpsAlt")
				{
					if (server.arg(i) != "")
					{
						config.gps_alt = server.arg(i).toFloat();
					}
				}

				if (server.argName(i) == "wifi_ssidAP")
				{
					if (server.arg(i) != "")
					{
						strcpy(config.wifi_ap_ssid, server.arg(i).c_str());
					}
				}
				if (server.argName(i) == "wifi_passAP")
				{
					if (server.arg(i) != "")
					{
						strcpy(config.wifi_ap_pass, server.arg(i).c_str());
					}
				}
				if (server.argName(i) == "wifi_ssid")
				{
					if (server.arg(i) != "")
					{
						strcpy(config.wifi_ssid, server.arg(i).c_str());
					}
				}
				if (server.argName(i) == "wifi_pass")
				{
					if (server.arg(i) != "")
					{
						strcpy(config.wifi_pass, server.arg(i).c_str());
					}
				}
				if (server.argName(i) == "wifi_pwr")
				{
					if (server.arg(i) != "")
					{
						if (isValidNumber(server.arg(i)))
							config.wifi_power = server.arg(i).toInt();
					}
				}
				if (server.argName(i) == "wifi_protocol")
				{
					if (server.arg(i) != "")
					{
						if (isValidNumber(server.arg(i)))
							config.wifi_protocol = server.arg(i).toInt();
					}
				}

				if (server.argName(i) == "oled_timeout")
				{
					if (server.arg(i) != "")
					{
						if (isValidNumber(server.arg(i)))
							config.oled_timeout = server.arg(i).toInt();
					}
				}
			}
			if (wifiAP && wifiSTA)
			{
				config.wifi_mode = WIFI_AP_STA_FIX;
			}
			else if (wifiAP)
			{
				config.wifi_mode = WIFI_AP_FIX;
			}
			else if (wifiSTA)
			{
				config.wifi_mode = WIFI_STA_FIX;
			}
			else
			{
				config.wifi_mode = WIFI_OFF_FIX;
			}
			config.oled_enable = oledEN;
			config.aprs = aprsEn;
			saveEEPROM();
			WiFi.disconnect();
			// #ifndef I2S_INTERNAL
			// 			if (adcIsr)
			// 				timerAlarmEnable(timer);
			// #endif
		}
	}

	setHTML(2);
	webString += "<div class=\"col-xs-10\">\n";
	webString += "<form accept-charset=\"UTF-8\" action=\"/config\" class=\"form-horizontal\" id=\"setting_form\" method=\"post\">\n";

	webString += "<div class = \"col-pad\">\n<h3>MY Location on APRS</h3>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">APRS Enable</label>\n";
	String aprsFlageEn = "";
	if (config.aprs == true)
		aprsFlageEn = "checked";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"field_checkbox\" id=\"aprsEnable\" name=\"aprsEnable\" type=\"checkbox\" value=\"OK\" " + aprsFlageEn + "/></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">Latitude(Deg.)</label>\n";
	webString += "<div class=\"col-sm-3 col-xs-6\"><input class=\"form-control\" id=\"gpsLat\" name=\"gpsLat\" type=\"text\" value=\"" + String(config.gps_lat, 5) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">Longitude(Deg.)</label>\n";
	webString += "<div class=\"col-sm-3 col-xs-6\"><input class=\"form-control\" id=\"gpsLon\" name=\"gpsLon\" type=\"text\" value=\"" + String(config.gps_lon, 5) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">Altitude(M)</label>\n";
	webString += "<div class=\"col-sm-3 col-xs-6\"><input class=\"form-control\" id=\"gpsAlt\" name=\"gpsAlt\" type=\"text\" value=\"" + String(config.gps_alt) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "</div>\n<hr>\n"; // div general

	webString += "<div class = \"col-pad\">\n<h3>OLED Display</h3>\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">OLED Enable</label>\n";
	String oledFlageEn = "";
	if (config.oled_enable == true)
		oledFlageEn = "checked";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"field_checkbox\" id=\"field_checkbox_0\" name=\"oledEnable\" type=\"checkbox\" value=\"OK\" " + oledFlageEn + "/></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">OLED Sleep</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><select name=\"oled_timeout\" id=\"oled_timeout\">\n";
	for (int i = 0; i < 600; i += 30)
	{
		if (config.oled_timeout == i)
			webString += "<option value=\"" + String(i) + "\" selected>" + String(i) + " Sec</option>\n";
		else
			webString += "<option value=\"" + String(i) + "\" >" + String(i) + " Sec</option>\n";
	}
	webString += "</select></div>\n";
	webString += "</div>\n";

	webString += "</div>\n<hr>\n"; // div display

	webString += "<div class = \"col-pad\">\n<h3>WiFi Network</h3>\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi AP Enable</label>\n";
	String wifiFlageAP = "";
	if ((config.wifi_mode == WIFI_AP_STA_FIX) || (config.wifi_mode == WIFI_AP_FIX))
		wifiFlageAP = "checked";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"field_checkbox\" id=\"field_checkbox_0\" name=\"wifiAP\" type=\"checkbox\" value=\"OK\" " + wifiFlageAP + "/></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi AP SSID</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"form-control\" id=\"wifi_ssidAP\" name=\"wifi_ssidAP\" type=\"text\" value=\"" + String(config.wifi_ap_ssid) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi AP PASSWORD</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"form-control\" id=\"wifi_passAP\" name=\"wifi_passAP\" type=\"password\" value=\"" + String(config.wifi_ap_pass) + "\" /></div>\n";
	webString += "</div><hr width=\"50%\">\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi Client Enable</label>\n";
	String wifiFlage = "";
	if ((config.wifi_mode == WIFI_AP_STA_FIX) || (config.wifi_mode == WIFI_STA_FIX))
		wifiFlage = "checked";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"field_checkbox\" id=\"field_checkbox_0\" name=\"wificlient\" type=\"checkbox\" value=\"OK\" " + wifiFlage + "/></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi Client SSID</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"form-control\" id=\"wifi_ssid\" name=\"wifi_ssid\" type=\"text\" value=\"" + String(config.wifi_ssid) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi Client PASSWORD</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><input class=\"form-control\" id=\"wifi_pass\" name=\"wifi_pass\" type=\"password\" value=\"" + String(config.wifi_pass) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi Mode</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><select name=\"wifi_protocol\" id=\"wifi_protocol\">\n";

	if (config.wifi_protocol == 7)
	{
		webString += "<option value=\"1\">IEEE 802.11b</option>\n";
		webString += "<option value=\"3\">IEEE 802.11bg</option>\n";
		webString += "<option value=\"7\" selected>IEEE 802.11bgn</option>\n";
	}
	else if (config.wifi_protocol == 3)
	{
		webString += "<option value=\"1\">IEEE 802.11b</option>\n";
		webString += "<option value=\"3\" selected>IEEE 802.11bg</option>\n";
		webString += "<option value=\"7\">IEEE 802.11bgn</option>\n";
	}
	else if (config.wifi_protocol == 1)
	{
		webString += "<option value=\"1\" selected>IEEE 802.11b</option>\n";
		webString += "<option value=\"3\">IEEE 802.11bg</option>\n";
		webString += "<option value=\"7\">IEEE 802.11bgn</option>\n";
	}
	else
	{
		config.wifi_protocol = 7;
		webString += "<option value=\"1\" selected>IEEE 802.11b</option>\n";
		webString += "<option value=\"3\">IEEE 802.11bg</option>\n";
		webString += "<option value=\"7\">IEEE 802.11bgn</option>\n";
	}
	webString += "</select></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\">WiFi Power</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-6\"><select name=\"wifi_pwr\" id=\"wifi_pwr\">\n";
	for (int i = 0; i < 12; i++)
	{
		if (config.wifi_power == wifiPwr[i][0])
			webString += "<option value=\"" + String(wifiPwr[i][0], 0) + "\" selected>" + String(wifiPwr[i][1], 1) + " dBm</option>\n";
		else
			webString += "<option value=\"" + String(wifiPwr[i][0], 0) + "\" >" + String(wifiPwr[i][1], 1) + " dBm</option>\n";
	}
	webString += "</select></div>\n";
	webString += "</div>\n";

	webString += "</div>\n<hr>\n"; // div network

	webString += "<div class = \"col-pad\">\n<h3>WiFi Status</h3>\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<table border=\"0\" cellspacing=\"1\" cellpadding=\"2\">\n";
	webString += "<tr><td align=\"right\"><b>Mode:</b></td>\n";
	webString += "<td align=\"left\">";

	if (config.wifi_mode == WIFI_AP_FIX)
	{
		webString += "AP";
	}
	else if (config.wifi_mode == WIFI_STA_FIX)
	{
		webString += "STA";
	}
	else if (config.wifi_mode == WIFI_AP_STA_FIX)
	{
		webString += "AP+STA";
	}
	else
	{
		webString += "OFF";
	}

	wifi_power_t wpr = WiFi.getTxPower();
	String wifipower = "";
	if (wpr < 8)
	{
		wifipower = "-1 dBm";
	}
	else if (wpr < 21)
	{
		wifipower = "2 dBm";
	}
	else if (wpr < 29)
	{
		wifipower = "5 dBm";
	}
	else if (wpr < 35)
	{
		wifipower = "8.5 dBm";
	}
	else if (wpr < 45)
	{
		wifipower = "11 dBm";
	}
	else if (wpr < 53)
	{
		wifipower = "13 dBm";
	}
	else if (wpr < 61)
	{
		wifipower = "15 dBm";
	}
	else if (wpr < 69)
	{
		wifipower = "17 dBm";
	}
	else if (wpr < 75)
	{
		wifipower = "18.5 dBm";
	}
	else if (wpr < 77)
	{
		wifipower = "19 dBm";
	}
	else if (wpr < 80)
	{
		wifipower = "19.5 dBm";
	}
	else
	{
		wifipower = "20 dBm";
	}

	webString += "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>MAC:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.macAddress()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Channel:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.channel()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>TX Power:</b></td>\n";
	webString += "<td align=\"left\">" + wifipower + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>SSID:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.SSID()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Local IP:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.localIP().toString() + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Gateway IP:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.gatewayIP().toString() + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>DNS:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.dnsIP().toString() + "</td></tr>\n";
	webString += "</table></div>\n";

	webString += "</div><hr>\n";

	webString += "</div>\n<hr>\n"; // div loger

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\"></label>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"btn btn-primary\" id=\"setting_form_sumbit\" name=\"commit\" type=\"submit\" value=\"Save Config\" maxlength=\"80\"/></div>\n";
	webString += "</form><form action=\"/default\" class=\"button_to\" method=\"post\">\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"btn btn-danger\" id=\"default_form_sumbit\" name=\"commit\" type=\"submit\" value=\"Default Config\" maxlength=\"80\"/></div>\n";
	webString += "</form>\n";
	webString += "</div>\n";

	webString += "</div>\n";

	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked
}

void handle_vpn()
{
	// byte *ptr;
	bool vpnEn = false;
	//	bool taretime = false;
	// bool davisEn = false;
	// bool moniEn = false;

	if (server.args() > 0)
	{
		//			taretime = false;
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));

			if (server.argName(i) == "vpnEnable")
			{
				if (server.arg(i) != "")
				{
					// if (isValidNumber(server.arg(i)))
					if (String(server.arg(i)) == "OK")
						vpnEn = true;
				}
			}

			// if (server.argName(i) == "taretime") {
			//	if (server.arg(i) != "")
			//	{
			//		//if (isValidNumber(server.arg(i)))
			//		if (String(server.arg(i)) == "OK")
			//			taretime = true;
			//	}
			// }
			if (server.argName(i) == "wg_port")
			{
				if (server.arg(i) != "")
				{
					config.wg_port = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "wg_public_key")
			{
				if (server.arg(i) != "")
				{
					strncpy(config.wg_public_key, server.arg(i).c_str(), 44);
					config.wg_public_key[44] = 0;
				}
			}

			if (server.argName(i) == "wg_private_key")
			{
				if (server.arg(i) != "")
				{
					strncpy(config.wg_private_key, server.arg(i).c_str(), 44);
					config.wg_private_key[44] = 0;
				}
			}

			if (server.argName(i) == "wg_peer_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_peer_address, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "wg_local_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_local_address, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "wg_netmask_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_netmask_address, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "wg_gw_address")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.wg_gw_address, server.arg(i).c_str());
				}
			}
		}

		config.vpn = vpnEn;
		saveEEPROM();
		// topBar(WiFi.RSSI());
	}

	// getMoisture();       // read sensor
	// webMessage = "";
	setHTML(8);
	webString += "<div class=\"col-xs-10\">\n";
	webString += "<form accept-charset=\"UTF-8\" action=\"/vpn\" class=\"form-horizontal\" id=\"setting_form\" method=\"post\">\n";

	webString += "<div class = \"\">\n<h3>Wireguard Configuration</h3>\n";

	String syncFlage = "";
	if (config.vpn)
		syncFlage = "checked";
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Enable</label>\n";
	webString += "<div class=\"col-sm-3 col-xs-6\"><input class=\"field_checkbox\" id=\"vpnEnable\" name=\"vpnEnable\" type=\"checkbox\" value=\"OK\" " + syncFlage + "/></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Server Address</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"wg_peer_address\" name=\"wg_peer_address\" type=\"text\" value=\"" + String(config.wg_peer_address) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Server Port</label>\n";
	webString += "<div class=\"col-sm-1 col-xs-2\"><input class=\"form-control\" id=\"wg_port\" name=\"wg_port\" type=\"text\" value=\"" + String(config.wg_port) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Local Address</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"wg_local_address\" name=\"wg_local_address\" type=\"text\" value=\"" + String(config.wg_local_address) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Netmask</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"wg_netmask_address\" name=\"wg_netmask_address\" type=\"text\" value=\"" + String(config.wg_netmask_address) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Gateway</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"wg_gw_address\" name=\"wg_gw_address\" type=\"text\" value=\"" + String(config.wg_gw_address) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Public Key</label>\n";
	webString += "<div class=\"col-sm-7 col-xs-10\"><input class=\"form-control\" id=\"wg_public_key\" name=\"wg_public_key\" type=\"text\" value=\"" + String(config.wg_public_key) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\">Private Key</label>\n";
	webString += "<div class=\"col-sm-7 col-xs-10\"><input class=\"form-control\" id=\"wg_private_key\" name=\"wg_private_key\" type=\"text\" value=\"" + String(config.wg_private_key) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "</div>\n"; // div general

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\"></label>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"btn btn-primary\" id=\"setting_form_sumbit\" name=\"commit\" type=\"submit\" value=\"Save Config\" maxlength=\"80\"/></div>\n";
	webString += "</form>\n";
	webString += "</div>\n";

	webString += "</div>\n";

	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked
	delay(100);
	webString.clear();
}

void handle_service()
{
	bool noiseEn = false;
	bool agcEn = false;
	bool dtmfEn = false;
	if (server.hasArg("commit"))
	{
		// #ifndef I2S_INTERNAL
		// 		bool adcIsr = timerAlarmEnabled(timer);
		// 		if (adcIsr)
		// 			timerAlarmDisable(timer);
		// #endif
		disconnect_from_host(); // Restart M17 to Reflector
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));

			if (server.argName(i) == "noiseCheck")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						noiseEn = true;
					}
				}
			}
			if (server.argName(i) == "agcCheck")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						agcEn = true;
					}
				}
			}
			if (server.argName(i) == "dtmfCheck")
			{
				if (server.arg(i) != "")
				{
					if (String(server.arg(i)) == "OK")
					{
						dtmfEn = true;
					}
				}
			}
			if (server.argName(i) == "m17Name")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.reflector_name, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "m17Host")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.reflector_host, server.arg(i).c_str());
				}
			}
			if (server.argName(i) == "m17Port")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.reflector_port = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "m17Module")
			{
				if (server.arg(i) != "")
				{
					config.reflector_module = server.arg(i).charAt(0);
					current_module = config.reflector_module;
				}
			}

			if (server.argName(i) == "myCallM17")
			{
				if (server.arg(i) != "")
				{
					strcpy(config.mycall, server.arg(i).c_str());
				}
			}

			if (server.argName(i) == "myModM17")
			{
				if (server.arg(i) != "")
				{
					config.mymodule = server.arg(i).charAt(0);
				}
			}

			if (server.argName(i) == "sql")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						if (server.arg(i).toInt())
							config.sql = true;
						else
							config.sql = false;
					}
				}
			}

			if (server.argName(i) == "sql_active")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						if (server.arg(i).toInt())
							config.sql_active = true;
						else
							config.sql_active = false;
					}
				}
			}
#ifndef SA818
			if (server.argName(i) == "vox_level")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.vox_level = server.arg(i).toInt();
				}
			}
#endif

			if (server.argName(i) == "mic_level")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.mic = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "vox_delay")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.vox_delay = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "codec2_mode")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.codec2_mode = server.arg(i).toInt();
						if (config.codec2_mode != 0)
							config.codec2_mode = 2; // CODEC2_MODE_1600 1600bps
					}
				}
			}
		}
		config.noise = noiseEn;
		config.agc = agcEn;
		config.dtmf = dtmfEn;
		saveEEPROM();
		// #ifndef I2S_INTERNAL
		// 		if (adcIsr)
		// 			timerAlarmDisable(timer);
		// #endif
	}

	setHTML(3);
	webString += "<div class=\"col-xs-10\">\n";
	webString += "<form accept-charset=\"UTF-8\" action=\"/service\" class=\"form-horizontal\" id=\"setting_form\" method=\"post\">\n";

	webString += "<div>\n<h3>Digital Voice</h3>\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">M17 Host Name</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"form-control\" id=\"m17Name\" name=\"m17Name\" type=\"text\" value=\"" + String(config.reflector_name) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">M17 Host</label>\n";
	webString += "<div class=\"col-sm-6 col-xs-8\"><input class=\"form-control\" id=\"m17Host\" name=\"m17Host\" type=\"text\" value=\"" + String(config.reflector_host) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">M17 Port</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"form-control\" id=\"m17Port\" name=\"m17Port\" type=\"text\" value=\"" + String(config.reflector_port) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">M17 Module/ROOM</label>\n";
	// webString += "<div class=\"col-sm-1 col-xs-2\"><input class=\"form-control\" id=\"m17Module\" name=\"m17Module\" type=\"text\" value=\"" + String(config.reflector_module) + "\" /></div>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"m17Module\" id=\"m17Module\">\n";
	for (char mod = 'A'; mod <= 'Z'; mod++)
	{
		if (config.reflector_module == mod)
		{
			webString += "<option value=\"" + String(mod) + "\" selected>" + String(mod) + "</option>\n";
		}
		else
		{
			webString += "<option value=\"" + String(mod) + "\">" + String(mod) + "</option>\n";
		}
	}
	webString += "</select></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">myCallSign</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"form-control\" id=\"myCallM17\" name=\"myCallM17\" type=\"text\" value=\"" + String(config.mycall) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">myModule/SSID</label>\n";
	// webString += "<div class=\"col-sm-1 col-xs-2\"><input class=\"form-control\" id=\"myModM17\" name=\"myModM17\" type=\"text\" value=\"" + String(config.mymodule) + "\" /></div>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"myModM17\" id=\"myModM17\">\n";
	for (char mod = 'A'; mod <= 'Z'; mod++)
	{
		if (config.mymodule == mod)
		{
			webString += "<option value=\"" + String(mod) + "\" selected>" + String(mod) + "</option>\n";
		}
		else
		{
			webString += "<option value=\"" + String(mod) + "\">" + String(mod) + "</option>\n";
		}
	}
	webString += "</select></div>\n";
	webString += "</div>\n";

	String cmSelSqlT = "";
	String cmSelSqlF = "";
	if (config.sql)
	{
		cmSelSqlT = "selected";
	}
	else
	{
		cmSelSqlF = "selected";
	}
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">Key PTT Mode</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"sql\" id=\"sql\">\n<option value=\"1\" " + cmSelSqlT + ">SQC Signal</option>\n<option value=\"0\" " + cmSelSqlF + ">VOX</option></select></div>\n";
	webString += "</div>\n";

	cmSelSqlT = "";
	cmSelSqlF = "";
	if (config.sql_active)
	{
		cmSelSqlT = "selected";
	}
	else
	{
		cmSelSqlF = "selected";
	}
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">SQC Active</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"sql_active\" id=\"sql_active\">\n<option value=\"1\" " + cmSelSqlT + ">HIGH</option>\n<option value=\"0\" " + cmSelSqlF + ">LOW</option></select></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">VOX/PTT DELAY</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"vox_delay\" name=\"vox_delay\" min=\"0\" max=\"200\" value=\"" + String(config.vox_delay) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";

#ifndef SA818
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">VOX Level</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"vox_level\" name=\"vox_level\" type=\"range\" min=\"0\" max=\"255\" value=\"" + String(config.vox_level) + "\" onchange=\"showVoxValue(this.value)\" /><span id=\"voxShow\">" + String(config.vox_level) + "</span></div>\n";
#endif
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">Audio->M17 Gain</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"mic_level\" name=\"mic_level\" type=\"range\" min=\"1\" max=\"20\" value=\"" + String(config.mic) + "\" onchange=\"showMicValue(this.value)\" /><span id=\"micShow\">" + String(config.mic) + "</span></div>\n";
	webString += "</div>\n";

	String cmSel3200 = "";
	String cmSel1600 = "";
	if (config.codec2_mode == 0)
	{
		cmSel3200 = "selected";
	}
	else
	{
		cmSel1600 = "selected";
	}
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">Codec2 Mode</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"codec2_mode\" id=\"codec2_mode\">\n<option value=\"0\" " + cmSel3200 + ">CODEC2_3200</option>\n<option value=\"2\" " + cmSel1600 + ">CODEC2_1600</option></select></div>\n";
	webString += "</div>\n";

	//#ifdef I2S_INTERNAL
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">Noise Cancel</label>\n";
	String noiseFlage = "";
	if (config.noise)
		noiseFlage = "checked";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"field_checkbox\" id=\"field_checkbox_1\" name=\"noiseCheck\" type=\"checkbox\" value=\"OK\" " + noiseFlage + "/></div>\n";
	webString += "</div>\n";
	//#endif

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">AGC</label>\n";
	String agcFlage = "";
	if (config.agc)
		agcFlage = "checked";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"field_checkbox\" id=\"field_checkbox_2\" name=\"agcCheck\" type=\"checkbox\" value=\"OK\" " + agcFlage + "/></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">DTMF</label>\n";
	String dtmfFlage = "";
	if (config.dtmf)
		dtmfFlage = "checked";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"field_checkbox\" id=\"dtmfCheck\" name=\"dtmfCheck\" type=\"checkbox\" value=\"OK\" " + dtmfFlage + "/></div>\n";
	webString += "</div>\n";

	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\"></label>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"btn btn-primary\" id=\"setting_form_sumbit\" name=\"commit\" type=\"submit\" value=\"Save Config\" maxlength=\"80\"/></div>\n";
	webString += "</div>\n";

	webString += "</form></div>\n";

	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked

	// delay(100);
}

#ifdef SA818
void handle_radio()
{
	// bool noiseEn=false;
	// bool agcEn=false;
	if (server.hasArg("commit"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));

			// if (server.argName(i) == "agcCheck")
			// {
			// 	if (server.arg(i) != "")
			// 		{
			// 			if (String(server.arg(i)) == "OK")
			// 			{
			// 				agcEn=true;
			// 			}
			// 		}
			// }

			if (server.argName(i) == "nw_band")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						config.band = server.arg(i).toInt();
						// if (server.arg(i).toInt())
						// 	config.band = 1;
						// else
						// 	config.band = 0;
					}
				}
			}

			if (server.argName(i) == "volume")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.volume = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "rf_power")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
					{
						if (server.arg(i).toInt())
							config.rf_power = true;
						else
							config.rf_power = false;
					}
				}
			}

			if (server.argName(i) == "sql_level")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.sql_level = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "tx_freq")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.freq_tx = server.arg(i).toFloat();
				}
			}
			if (server.argName(i) == "rx_freq")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.freq_rx = server.arg(i).toFloat();
				}
			}

			if (server.argName(i) == "tx_offset")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.offset_tx = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "rx_offset")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.offset_rx = server.arg(i).toInt();
				}
			}

			if (server.argName(i) == "tx_ctcss")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.tone_tx = server.arg(i).toInt();
				}
			}
			if (server.argName(i) == "rx_ctcss")
			{
				if (server.arg(i) != "")
				{
					if (isValidNumber(server.arg(i)))
						config.tone_rx = server.arg(i).toInt();
				}
			}
		}
		// config.noise=noiseEn;
		// config.agc=agcEn;
		saveEEPROM();
		// delay(100);
		SA818_INIT(false);
	}

	setHTML(6);
	webString += "<div class=\"col-xs-10\">\n";
	webString += "<form accept-charset=\"UTF-8\" action=\"/radio\" class=\"form-horizontal\" id=\"radio_form\" method=\"post\">\n";

	webString += "<div>\n<h3>RF Module SA818/SA868</h3>\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">TX Frequency</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"tx_freq\" name=\"tx_freq\" min=\"144.0000\" max=\"148.0000\" step=\"0.0125\" value=\"" + String(config.freq_tx, 4) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">RX Frequency</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"rx_freq\" name=\"rx_freq\" min=\"144.0000\" max=\"148.0000\" step=\"0.0125\" value=\"" + String(config.freq_rx, 4) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">TX Offset</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"tx_offset\" name=\"tx_offset\" min=\"-5000\" max=\"5000\" step=\"100\" value=\"" + String(config.offset_tx) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">RX Offset</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"rx_offset\" name=\"rx_offset\" min=\"-5000\" max=\"5000\" step=\"100\" value=\"" + String(config.offset_rx) + "\" /></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">TX CTCSS</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"tx_ctcss\" id=\"tx_ctcss\">\n";
	for (int i = 0; i < 39; i++)
	{
		if (config.tone_tx == i)
			webString += "<option value=\"" + String(i) + "\" selected>" + String(ctcss[i], 1) + "</option>\n";
		else
			webString += "<option value=\"" + String(i) + "\" >" + String(ctcss[i], 1) + "</option>\n";
	}
	webString += "</select></div>\n";
	webString += "</div>\n";
	// webString += "<div class=\"form-group\">\n";
	// webString += "<label class=\"col-sm-3 col-xs-12 control-label\">TX CTCSS Ch</label>\n";
	// webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"tx_ctcss\" name=\"tx_ctcss\" min=\"0\" max=\"38\" step=\"1\" value=\"" + String(config.tone_tx) + "\" /></div>\n";
	// webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">RX CTCSS</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"rx_ctcss\" id=\"rx_ctcss\">\n";
	for (int i = 0; i < 39; i++)
	{
		if (config.tone_rx == i)
			webString += "<option value=\"" + String(i) + "\" selected>" + String(ctcss[i], 1) + "</option>\n";
		else
			webString += "<option value=\"" + String(i) + "\" >" + String(ctcss[i], 1) + "</option>\n";
	}
	webString += "</select></div>\n";
	webString += "</div>\n";

	// webString += "<div class=\"form-group\">\n";
	// webString += "<label class=\"col-sm-3 col-xs-12 control-label\">RX CTCSS Ch</label>\n";
	// webString += "<div class=\"col-sm-2 col-xs-6\"><input type=\"number\" id=\"rx_ctcss\" name=\"rx_ctcss\" min=\"0\" max=\"38\" step=\"1\" value=\"" + String(config.tone_rx) + "\" /></div>\n";
	// webString += "</div>\n";

	String cmSelSqlT = "";
	String cmSelSqlF = "";
	if (config.band)
	{
		cmSelSqlT = "selected";
	}
	else
	{
		cmSelSqlF = "selected";
	}
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">Narrow/Wide</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"nw_band\" id=\"nw_band\">\n<option value=\"1\" " + cmSelSqlT + ">25.0KHz</option>\n<option value=\"0\" " + cmSelSqlF + ">12.5KHz</option></select></div>\n";
	webString += "</div>\n";

	cmSelSqlF = "";
	cmSelSqlT = "";
	if (config.rf_power)
	{
		cmSelSqlT = "selected";
	}
	else
	{
		cmSelSqlF = "selected";
	}
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">RF Power</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><select name=\"rf_power\" id=\"rf_power\">\n<option value=\"1\" " + cmSelSqlT + ">HIGH</option>\n<option value=\"0\" " + cmSelSqlF + ">LOW</option></select></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">VOLUME</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"volume\" name=\"volume\" type=\"range\" min=\"1\" max=\"8\" value=\"" + String(config.volume) + "\" onchange=\"showVoxValue(this.value)\" /><span id=\"voxShow\">" + String(config.volume) + "</span></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-3 col-xs-12 control-label\">SQL Level</label>\n";
	webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"sql_level\" name=\"sql_level\" type=\"range\" min=\"0\" max=\"8\" value=\"" + String(config.sql_level) + "\" onchange=\"showSqlValue(this.value)\" /><span id=\"sqlShow\">" + String(config.sql_level) + "</span></div>\n";
	webString += "</div>\n";

	// webString += "<div class=\"form-group\">\n";
	// webString += "<label class=\"col-sm-3 col-xs-12 control-label\">MIC Gain</label>\n";
	// webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"form-control\" id=\"mic_level\" name=\"mic_level\" type=\"range\" min=\"1\" max=\"20\" value=\"" + String(config.mic) + "\" onchange=\"showMicValue(this.value)\" /><span id=\"micShow\">" + String(config.mic) + "</span></div>\n";
	// webString += "</div>\n";

	// webString += "<div class=\"form-group\">\n";
	// webString += "<label class=\"col-sm-3 col-xs-12 control-label\">AGC</label>\n";
	// String agcFlage = "";
	// if (config.agc)
	// 	agcFlage = "checked";
	// webString += "<div class=\"col-sm-2 col-xs-6\"><input class=\"field_checkbox\" id=\"field_checkbox_2\" name=\"agcCheck\" type=\"checkbox\" value=\"OK\" " + agcFlage + "/></div>\n";
	// webString += "</div>\n";

	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-4 col-xs-12 control-label\"></label>\n";
	// webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"btn btn-primary\" id=\"radio_set_sumbit\" name=\"commit\" type=\"submit\" value=\"SET\" maxlength=\"80\"/></div>\n";
	webString += "<div class=\"col-sm-2 col-xs-4\"><input class=\"btn btn-primary\" id=\"radio_form_sumbit\" name=\"commit\" type=\"submit\" value=\"Save Config\" maxlength=\"80\"/></div>\n";
	webString += "</div>\n";

	webString += "</form></div>\n";

	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked

	// delay(100);
}
#endif

void handle_system()
{
	if (server.hasArg("updateTimeNtp"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "SetTimeNtp")
			{
				if (server.arg(i) != "")
				{
					Serial.println("WEB Config NTP");
					configTime(3600 * config.timeZone, 0, server.arg(i).c_str());
				}
				break;
			}
		}
	}
	else if (server.hasArg("updateTime"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "SetTime")
			{
				if (server.arg(i) != "")
				{
					// struct tm tmn;
					String date = getValue(server.arg(i), ' ', 0);
					String time = getValue(server.arg(i), ' ', 1);
					int yyyy = getValue(date, '-', 0).toInt();
					int mm = getValue(date, '-', 1).toInt();
					int dd = getValue(date, '-', 2).toInt();
					int hh = getValue(time, ':', 0).toInt();
					int ii = getValue(time, ':', 1).toInt();
					int ss = getValue(time, ':', 2).toInt();
					// int ss = 0;

					tmElements_t timeinfo;
					timeinfo.Year = yyyy - 1970;
					timeinfo.Month = mm;
					timeinfo.Day = dd;
					timeinfo.Hour = hh;
					timeinfo.Minute = ii;
					timeinfo.Second = ss;
					time_t timeStamp = makeTime(timeinfo);

					// tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec

					time_t rtc = timeStamp - 25200;
					timeval tv = {rtc, 0};
					timezone tz = {TZ_SEC + DST_MN, 0};
					settimeofday(&tv, &tz);

					// Serial.println("Update TIME " + server.arg(i));
					Serial.print("Set New Time at ");
					Serial.print(dd);
					Serial.print("/");
					Serial.print(mm);
					Serial.print("/");
					Serial.print(yyyy);
					Serial.print(" ");
					Serial.print(hh);
					Serial.print(":");
					Serial.print(ii);
					Serial.print(":");
					Serial.print(ss);
					Serial.print(" ");
					Serial.println(timeStamp);
				}
				break;
			}
		}
	}
	else if (server.hasArg("updateTimeZone"))
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			// Serial.print("SERVER ARGS ");
			// Serial.print(server.argName(i));
			// Serial.print("=");
			// Serial.println(server.arg(i));
			if (server.argName(i) == "SetTimeZone")
			{
				if (server.arg(i) != "")
				{
					config.timeZone = atoi(server.arg(i).c_str());
					// Serial.println("WEB Config NTP");
					configTime(3600 * config.timeZone, 0, "203.150.19.26");
					// configTime(3600 * timeZone, 0, server.arg(i).c_str());
				}
				break;
			}
		}
		saveEEPROM();
	}
	else if (server.hasArg("REBOOT"))
	{
		esp_restart();
	}

	struct tm tmstruct;
	char strTime[20];
	tmstruct.tm_year = 0;
	getLocalTime(&tmstruct, 5000);
	sprintf(strTime, "%d-%02d-%02d %02d:%02d:%02d", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);

	// webMessage = "";
	setHTML(4);
	// webString += "<div class=\"col-xs-12\">\n";
	webString += "<script type=\"text/javascript\" src=\"//code.jquery.com/jquery-2.1.1.min.js\"></script>\n";
	webString += "<script type=\"text/javascript\" src=\"//maxcdn.bootstrapcdn.com/bootstrap/3.3.1/js/bootstrap.min.js\"></script>\n";
	webString += "<script src=\"//cdnjs.cloudflare.com/ajax/libs/moment.js/2.9.0/moment-with-locales.js\"></script>\n";
	webString += "<script src=\"//cdn.rawgit.com/Eonasdan/bootstrap-datetimepicker/e8bddc60e73c1ec2475f827be36e1957af72e2ea/src/js/bootstrap-datetimepicker.js\"></script>";

	webString += "<div class=\"col-pad\">\n<h3>TIME Setting</h3>\n<table border=\"0\"><tr>";

	webString += "<div class=\"form-group\">\n";
	webString += "<td><label class=\"col-sm-2 col-xs-12 control-label\">Current</label></td>\n";
	webString += "<td>" + String(strTime) + "</td>\n";
	webString += "</div>\n</tr><tr>";

	webString += "<form accept-charset=\"UTF-8\" action=\"/system\" class=\"form-horizontal\" id=\"time_form_zone\" method=\"post\">\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<td><label class=\"col-sm-8 col-xs-12 control-label\">TIME Zone</label></td>\n";
	webString += "<td><div class=\"col-sm-2 col-xs-4\" id='time_zone'><input class=\"form-control\" name=\"SetTimeZone\" type=\"text\" value=\"" + String(config.timeZone) + ""
																																											 "\" />\n";
	webString += "</div></td>\n";
	webString += "<td><input class=\"btn btn-primary\" id=\"setting_time_sumbit\" name=\"updateTimeZone\" type=\"submit\" value=\"Update\" maxlength=\"80\"/></td>\n";
	webString += "</div>\n</form>\n</tr><tr>\n";

	webString += "<form accept-charset=\"UTF-8\" action=\"/system\" class=\"form-horizontal\" id=\"time_form\" method=\"post\">\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<td><label class=\"col-sm-2 col-xs-12 control-label\">DATE/TIME</label></td>\n";
	webString += "<td><div class=\"input-group date\" id='datetimepicker1'><input class=\"form-control\" name=\"SetTime\" type=\"text\" value=\"" + String(strTime) + "\" />\n";
	webString += "<span class=\"input-group-addon\">\n<span class=\"glyphicon glyphicon-calendar\">\n</span></span></div></td>\n";
	// webString += "<div class=\"col-sm-3 col-xs-6\"><button class=\"btn btn-primary\" data-args=\"[true]\" data-method=\"getDate\" type=\"button\" data-related-target=\"#SetTime\" />Get Date</button></div>\n";
	webString += "<td><input class=\"btn btn-primary\" id=\"setting_time_sumbit\" name=\"updateTime\" type=\"submit\" value=\"Time Update\" maxlength=\"80\"/></td>\n";
	webString += "</div>\n</form>\n</tr><tr>\n";

	webString += "<form accept-charset=\"UTF-8\" action=\"/system\" class=\"form-horizontal\" id=\"time_form_ntp\" method=\"post\">\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<td><label class=\"col-sm-2 col-xs-12 control-label\">NTP_Host</label></td>\n";
	webString += "<td><div class=\"input-group\" id='ntp_update'><input class=\"form-control\" name=\"SetTimeNtp\" type=\"text\" value=\"203.150.19.26\" />\n";
	webString += "</div></td>\n";
	webString += "<td><input class=\"btn btn-primary\" id=\"setting_time_sumbit\" name=\"updateTimeNtp\" type=\"submit\" value=\"NTP Update\" maxlength=\"80\"/></td>\n";
	webString += "</div>\n</form>\n</tr><tr>\n";

	webString += "<form accept-charset=\"UTF-8\" action=\"/system\" class=\"form-horizontal\" id=\"reboot_form\" method=\"post\">\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<td><label class=\"col-sm-2 col-xs-12 control-label\">SYSTEM</label></td>\n";
	webString += "<td><input type='submit' class=\"btn btn-danger\" name=\"REBOOT\" value='REBOOT'></td>\n";
	webString += "</div>\n</form>\n</tr></table>\n";

	webString += "</div><hr>\n";
	webString += "<div class = \"col-pad\">\n<h3>WiFi Status</h3>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<table border=\"0\" cellspacing=\"10\" cellpadding=\"10\">\n";
	webString += "<tr><td align=\"right\"><b>Mode:</b></td>\n";
	webString += "<td align=\"left\">";
	if (config.wifi_mode == WIFI_AP_FIX)
	{
		webString += "AP";
	}
	else if (config.wifi_mode == WIFI_STA_FIX)
	{
		webString += "STA";
	}
	else if (config.wifi_mode == WIFI_AP_STA_FIX)
	{
		webString += "AP+STA";
	}
	else
	{
		webString += "OFF";
	}
	webString += "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>MAC:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.macAddress()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Channel:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.channel()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>TX Power:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.getTxPower()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>SSID:</b></td>\n";
	webString += "<td align=\"left\">" + String(WiFi.SSID()) + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Local IP:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.localIP().toString() + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>Gateway IP:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.gatewayIP().toString() + "</td></tr>\n";
	webString += "<tr><td align=\"right\"><b>DNS:</b></td>\n";
	webString += "<td align=\"left\">" + WiFi.dnsIP().toString() + "</td></tr>\n";
	webString += "</table></div>\n";

	webString += "</div><hr>\n";

	webString += "<script type=\"text/javascript\">\n"
				 "$(function(){\n"
				 "$('#datetimepicker1').datetimepicker({\n"
				 "locale: moment.locale('th'),\n"
				 "format: 'YYYY-MM-DD HH:mm:ss'\n"
				 "});\n"
				 "});\n</script>\n";
	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked

	// delay(100);
}

void handle_firmware()
{
	char strCID[50];
	uint64_t chipid = ESP.getEfuseMac();
	sprintf(strCID, "%04X%08X", (uint16_t)(chipid >> 32), (uint32_t)chipid);
	// webMessage = "";
	setHTML(5);

	webString += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>\n";
	webString += "<b>Current Hardware Version:</b> ESP32DR";
#ifdef SA818
#ifdef SR_FRS
	webString += " <b>(MODEL:SR_FRS_1W)</b>";
#else
	webString += " <b>(MODEL:SA818/SA868)</b>";
#endif
#else
	webString += " <b>(MODEL: Simple)</b>";
#endif
	webString += "<br /><b>Current Firmware Version:</b> V" + String(VERSION) + "\n<br/>";
	webString += "<b>Develop by:</b> HS5TQA\n<br />";
	webString += "<b>Chip ID:</b> " + String(strCID) + "\n<hr>";
	webString += "<div class = \"col-pad\">\n<h3>Firmware Update</h3>\n";
	webString += "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form' class=\"form-horizontal\">\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-6 control-label\">FILE</label>\n";
	webString += "<div class=\"col-sm-4 col-xs-12\"><input id=\"file\" name=\"update\" type=\"file\" onchange='sub(this)' /></div>\n";
	// webString += "<div class=\"col-sm-4 col-xs-12\"><label id='file-input' for='file'>   Choose file...</label></div>\n";
	// webString += "<div class=\"col-sm-3 col-xs-4\"><input type='submit' class=\"btn btn-danger\" id=\"update_sumbit\" value='Firmware Update'></div>\n";
	webString += "</div>\n";
	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\"></label>\n";
	webString += "<div class=\"col-sm-3 col-xs-4\"><input type='submit' class=\"btn btn-danger\" id=\"update_sumbit\" value='Firmware Update'></div>\n";
	webString += "</div>\n";

	webString += "<div class=\"form-group\">\n";
	webString += "<label class=\"col-sm-2 col-xs-12 control-label\"></label>\n";
	webString += "<div id='prg'></div>\n";
	webString += "<br><div id='prgbar'><div id='bar'></div></div>\n";
	webString += "</div>\n";

	webString += "</form></div>\n";
	webString += "<script>"
				 "function sub(obj){"
				 "var fileName = obj.value.split('\\\\');"
				 "document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
				 "};"
				 "$('form').submit(function(e){"
				 "e.preventDefault();"
				 "var form = $('#upload_form')[0];"
				 "var data = new FormData(form);"
				 "$.ajax({"
				 "url: '/update',"
				 "type: 'POST',"
				 "data: data,"
				 "contentType: false,"
				 "processData:false,"
				 "xhr: function() {"
				 "var xhr = new window.XMLHttpRequest();"
				 "xhr.upload.addEventListener('progress', function(evt) {"
				 "if (evt.lengthComputable) {"
				 "var per = evt.loaded / evt.total;"
				 "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
				 "$('#bar').css('width',Math.round(per*100) + '%');"
				 "}"
				 "}, false);"
				 "return xhr;"
				 "},"
				 "success:function(d, s) {"
				 "console.log('success!') "
				 "},"
				 "error: function (a, b, c) {"
				 "}"
				 "});"
				 "});"
				 "</script>";

	webString += "</body></html>\n";
	server.send(200, "text/html", webString); // send to someones browser when asked

	// delay(100);
}

void handle_default()
{
	defaultSetting = true;
	defaultConfig();
	// webMessage = "";
	handle_setting();
	defaultSetting = false;
}

#ifdef SDCARD
void handle_download()
{
	String dataType = "text/plain";
	String path;
	if (server.args() > 0)
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "FILE")
			{
				path = server.arg(i);
				break;
			}
		}
	}

	if (path.endsWith(".src"))
		path = path.substring(0, path.lastIndexOf("."));
	else if (path.endsWith(".htm"))
		dataType = "text/html";
	else if (path.endsWith(".csv"))
		dataType = "text/csv";
	else if (path.endsWith(".css"))
		dataType = "text/css";
	else if (path.endsWith(".xml"))
		dataType = "text/xml";
	else if (path.endsWith(".png"))
		dataType = "image/png";
	else if (path.endsWith(".gif"))
		dataType = "image/gif";
	else if (path.endsWith(".jpg"))
		dataType = "image/jpeg";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".svg"))
		dataType = "image/svg+xml";
	else if (path.endsWith(".ico"))
		dataType = "image/x-icon";
	else if (path.endsWith(".js"))
		dataType = "application/javascript";
	else if (path.endsWith(".pdf"))
		dataType = "application/pdf";
	else if (path.endsWith(".zip"))
		dataType = "application/zip";
	else if (path.endsWith(".gz"))
	{
		if (path.startsWith("/gz/htm"))
			dataType = "text/html";
		else if (path.startsWith("/gz/css"))
			dataType = "text/css";
		else if (path.startsWith("/gz/csv"))
			dataType = "text/csv";
		else if (path.startsWith("/gz/xml"))
			dataType = "text/xml";
		else if (path.startsWith("/gz/js"))
			dataType = "application/javascript";
		else if (path.startsWith("/gz/svg"))
			dataType = "image/svg+xml";
		else
			dataType = "application/x-gzip";
	}
	// webString = "<html><head>\n";
	// webString += "<meta http - equiv = \"content-type\" content = \"text/html; charset=utf-8\" / > \n";
	// webMessage += "</head><body>\n";
	// webString += "</body></html>\n";
	File myFile = SD.open("/" + path, "r");
	if (myFile)
	{
		server.sendHeader("Content-Type", dataType);
		server.sendHeader("Content-Disposition", "attachment; filename=" + path);
		server.sendHeader("Connection", "close");
		server.streamFile(myFile, "application/octet-stream");
		myFile.close();
	}
	delay(100);
}

void handle_delete()
{
	String dataType = "text/plain";
	String path;
	if (server.args() > 0)
	{
		for (uint8_t i = 0; i < server.args(); i++)
		{
			if (server.argName(i) == "FILE")
			{
				path = server.arg(i);
				Serial.println("Deleting file: " + path);
				if (SD.remove("/" + path))
				{
					Serial.println("File deleted");
				}
				else
				{
					Serial.println("Delete failed");
				}
				break;
			}
		}
	}

	handle_storage();
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		Serial.println("Failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		Serial.println("Not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			Serial.print("  DIR : ");
			Serial.print(file.name());
			time_t t = file.getLastWrite();
			struct tm *tmstruct = localtime(&t);
			Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
			if (levels)
			{
				listDir(fs, file.name(), levels - 1);
			}
		}
		else
		{
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("  SIZE: ");
			Serial.print(file.size());
			time_t t = file.getLastWrite();
			struct tm *tmstruct = localtime(&t);
			Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
		}
		file = root.openNextFile();
	}
}
#endif

void webService()
{
	server.close();
	// web client handlers
	server.on("/", handle_root);
	server.on("/config", handle_setting);
#ifdef SDCARD
	server.on("/data", handle_storage);
	server.on("/download", handle_download);
	server.on("/delete", handle_delete);
#endif
#ifdef SA818
	server.on("/radio", handle_radio);
#endif
	server.on("/vpn", handle_vpn);
	server.on("/default", handle_default);
	server.on("/service", handle_service);
	server.on("/system", handle_system);
	server.on("/firmware", handle_firmware);
	/*handling uploading firmware file */
	server.on(
		"/update", HTTP_POST, []()
		{
			server.sendHeader("Connection", "close");
			server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
			ESP.restart(); },
		[]()
		{
			HTTPUpload &upload = server.upload();
			if (upload.status == UPLOAD_FILE_START)
			{
				// Serial.printf("Firmware Update FILE: %s\n", upload.filename.c_str());
				if (!Update.begin(UPDATE_SIZE_UNKNOWN))
				{ // start with max available size
					Update.printError(Serial);
					delay(3);
				}
				else
				{

					// //wdtDisplayTimer = millis();
					// //wdtSensorTimer = millis();
					// disableCore0WDT();
					// disableCore1WDT();
					// disableLoopWDT();
					// vTaskSuspend(taskDSPHandle);
					// //vTaskSuspend(taskUIHandle);
					// //vTaskSuspend(taskNetworkHandle);
					// config.aprs = false;
					frmUpdate(upload.filename);
					delay(3);
				}
			}
			else if (upload.status == UPLOAD_FILE_WRITE)
			{
				/* flashing firmware to ESP*/
				if (Update.write(upload.buf, upload.currentSize) != upload.currentSize)
				{
					Update.printError(Serial);
					delay(3);
				}
			}
			else if (upload.status == UPLOAD_FILE_END)
			{
				if (Update.end(true))
				{ // true to set the size to the current progress
					delay(3);
				}
				else
				{
					Update.printError(Serial);
					delay(3);
				}
			}
		});
	server.begin();
}