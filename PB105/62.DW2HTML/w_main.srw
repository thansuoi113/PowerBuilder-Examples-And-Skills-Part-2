$PBExportHeader$w_main.srw
forward
global type w_main from window
end type
type cb_exit from commandbutton within w_main
end type
type cb_saveas from commandbutton within w_main
end type
type cb_rtv1 from commandbutton within w_main
end type
type dw_1 from datawindow within w_main
end type
end forward

global type w_main from window
integer width = 2533
integer height = 1408
boolean titlebar = true
string title = "Untitled"
boolean controlmenu = true
boolean minbox = true
boolean maxbox = true
boolean resizable = true
long backcolor = 67108864
string icon = "AppIcon!"
boolean center = true
cb_exit cb_exit
cb_saveas cb_saveas
cb_rtv1 cb_rtv1
dw_1 dw_1
end type
global w_main w_main

on w_main.create
this.cb_exit=create cb_exit
this.cb_saveas=create cb_saveas
this.cb_rtv1=create cb_rtv1
this.dw_1=create dw_1
this.Control[]={this.cb_exit,&
this.cb_saveas,&
this.cb_rtv1,&
this.dw_1}
end on

on w_main.destroy
destroy(this.cb_exit)
destroy(this.cb_saveas)
destroy(this.cb_rtv1)
destroy(this.dw_1)
end on

type cb_exit from commandbutton within w_main
integer x = 1952
integer y = 132
integer width = 343
integer height = 128
integer taborder = 30
integer textsize = -12
integer weight = 400
fontcharset fontcharset = ansi!
fontpitch fontpitch = variable!
fontfamily fontfamily = swiss!
string facename = "Arial"
string text = "退出"
end type

event clicked;close(parent)
end event

type cb_saveas from commandbutton within w_main
integer x = 1051
integer y = 136
integer width = 357
integer height = 128
integer taborder = 20
integer textsize = -12
integer weight = 400
fontcharset fontcharset = ansi!
fontpitch fontpitch = variable!
fontfamily fontfamily = swiss!
string facename = "Arial"
string text = "另存为"
end type

event clicked;int ret

ret=dw_1.saveas("",Htmltable!,true)

if ret>0 then
	messagebox("提示!","存储成功，请到指定位置察看")
else
	messagebox("提示!","存储失败，请重新选择存储文件")
end if
return
end event

type cb_rtv1 from commandbutton within w_main
integer x = 283
integer y = 124
integer width = 357
integer height = 128
integer taborder = 10
integer textsize = -12
integer weight = 400
fontcharset fontcharset = ansi!
fontpitch fontpitch = variable!
fontfamily fontfamily = swiss!
string facename = "Arial"
string text = "查询"
end type

event clicked;
dw_1.reset()

dw_1.settransobject(sqlca)
dw_1.retrieve()

end event

type dw_1 from datawindow within w_main
integer y = 292
integer width = 2455
integer height = 988
integer taborder = 10
string title = "none"
string dataobject = "dw_customer"
boolean livescroll = true
borderstyle borderstyle = stylelowered!
end type

