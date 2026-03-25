; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.GtkWidget = type { ptr }
%struct.GtkWindow = type { %struct.GtkWidget }
%struct.GtkApplication = type { ptr }
%struct.GtkBox = type { %struct.GtkWidget }
%struct.GtkButton = type { %struct.GtkWidget }

@anon.string.0 = private unnamed_addr constant [14 x i8] c"Hello World!\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [24 x i8] c"Do you want to continue\00", align 4
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Cancel\00", align 4
@anon.string.3 = private unnamed_addr constant [4 x i8] c"Yes\00", align 4
@anon.string.4 = private unnamed_addr constant [22 x i8] c"com.spicelang.Example\00", align 4

define private void @_Z11btnYesClick9GtkWidget(%struct.GtkWidget noundef %0) {
  %widget = alloca %struct.GtkWidget, align 8
  store %struct.GtkWidget %0, ptr %widget, align 8
  call void @_Z6gPrintPKc(ptr noundef @anon.string.0)
  ret void
}

declare void @_Z6gPrintPKc(ptr)

define private void @_Z13destroyWindow9GtkWidget9GtkWindow(%struct.GtkWidget noundef %0, %struct.GtkWindow noundef %1) {
  %widget = alloca %struct.GtkWidget, align 8
  %window = alloca %struct.GtkWindow, align 8
  store %struct.GtkWidget %0, ptr %widget, align 8
  store %struct.GtkWindow %1, ptr %window, align 8
  call void @_ZN9GtkWindow7destroyEv(ptr noundef nonnull align 8 dereferenceable(8) %window)
  ret void
}

declare void @_ZN9GtkWindow7destroyEv(ptr)

define private void @_Z8activate14GtkApplicationPh(%struct.GtkApplication noundef %0, ptr noundef nonnull align 1 dereferenceable(1) %1) {
  %app = alloca %struct.GtkApplication, align 8
  %data = alloca ptr, align 8
  %window = alloca %struct.GtkWindow, align 8
  %box = alloca %struct.GtkBox, align 8
  %btnCancel = alloca %struct.GtkButton, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %btnYes = alloca %struct.GtkButton, align 8
  %fat.ptr1 = alloca { ptr, ptr }, align 8
  store %struct.GtkApplication %0, ptr %app, align 8
  store ptr %1, ptr %data, align 8
  %3 = call noundef %struct.GtkWindow @_ZN14GtkApplication9newWindowEv(ptr noundef nonnull align 8 dereferenceable(8) %app)
  store %struct.GtkWindow %3, ptr %window, align 8
  call void @_ZN9GtkWindow8setTitleEPKc(ptr noundef nonnull align 8 dereferenceable(8) %window, ptr noundef @anon.string.1)
  call void @_ZN9GtkWindow14setDefaultSizeEii(ptr noundef nonnull align 8 dereferenceable(8) %window, i32 noundef 350, i32 noundef 140)
  call void @_ZN6GtkBox4ctorEi(ptr noundef nonnull align 8 dereferenceable(8) %box, i32 noundef 0)
  call void @_ZN6GtkBox9setHAlignEi(ptr noundef nonnull align 8 dereferenceable(8) %box, i32 noundef 3)
  call void @_ZN6GtkBox9setVAlignEi(ptr noundef nonnull align 8 dereferenceable(8) %box, i32 noundef 3)
  %4 = load %struct.GtkBox, ptr %box, align 8
  call void @_ZN9GtkWindow8setChildE6GtkBox(ptr noundef nonnull align 8 dereferenceable(8) %window, %struct.GtkBox noundef %4)
  call void @_ZN9GtkButton4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(8) %btnCancel, ptr noundef @anon.string.2)
  store ptr @_Z13destroyWindow9GtkWidget9GtkWindow, ptr %fat.ptr, align 8
  %5 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %5, align 8
  %6 = load { ptr, ptr }, ptr %fat.ptr, align 8
  %7 = load %struct.GtkWindow, ptr %window, align 8
  call void @_ZN9GtkButton18setOnClickCallbackI9GtkWindowEEvPFv9GtkWidget9GtkWindowE9GtkWindow(ptr noundef nonnull align 8 dereferenceable(8) %btnCancel, { ptr, ptr } noundef %6, %struct.GtkWindow noundef %7)
  %8 = load %struct.GtkButton, ptr %btnCancel, align 8
  call void @_ZN6GtkBox6appendE9GtkButton(ptr noundef nonnull align 8 dereferenceable(8) %box, %struct.GtkButton noundef %8)
  call void @_ZN9GtkButton4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(8) %btnYes, ptr noundef @anon.string.3)
  store ptr @_Z11btnYesClick9GtkWidget, ptr %fat.ptr1, align 8
  %9 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 1
  store ptr poison, ptr %9, align 8
  %10 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  call void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr noundef nonnull align 8 dereferenceable(8) %btnYes, { ptr, ptr } noundef %10)
  %11 = load %struct.GtkButton, ptr %btnYes, align 8
  call void @_ZN6GtkBox6appendE9GtkButton(ptr noundef nonnull align 8 dereferenceable(8) %box, %struct.GtkButton noundef %11)
  call void @_ZN9GtkWindow7presentEv(ptr noundef nonnull align 8 dereferenceable(8) %window)
  ret void
}

declare %struct.GtkWindow @_ZN14GtkApplication9newWindowEv(ptr)

declare void @_ZN9GtkWindow8setTitleEPKc(ptr, ptr)

declare void @_ZN9GtkWindow14setDefaultSizeEii(ptr, i32, i32)

declare void @_ZN6GtkBox4ctorEi(ptr, i32)

declare void @_ZN6GtkBox9setHAlignEi(ptr, i32)

declare void @_ZN6GtkBox9setVAlignEi(ptr, i32)

declare void @_ZN9GtkWindow8setChildE6GtkBox(ptr, %struct.GtkBox)

declare void @_ZN9GtkButton4ctorEPKc(ptr, ptr)

declare void @_ZN9GtkButton18setOnClickCallbackI9GtkWindowEEvPFv9GtkWidget9GtkWindowE9GtkWindow(ptr, { ptr, ptr }, %struct.GtkWindow)

declare void @_ZN6GtkBox6appendE9GtkButton(ptr, %struct.GtkButton)

declare void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr, { ptr, ptr })

declare void @_ZN9GtkWindow7presentEv(ptr)

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main(i32 %0, ptr %1) #0 {
  %result = alloca i32, align 4
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
  %app = alloca %struct.GtkApplication, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  store i32 %0, ptr %argc, align 4
  store ptr %1, ptr %argv, align 8
  call void @_ZN14GtkApplication4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(8) %app, ptr noundef @anon.string.4)
  store ptr @_Z8activate14GtkApplicationPh, ptr %fat.ptr, align 8
  %3 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %3, align 8
  %4 = load { ptr, ptr }, ptr %fat.ptr, align 8
  call void @_ZN14GtkApplication19setActivateCallbackEPFv14GtkApplicationPhE(ptr noundef nonnull align 8 dereferenceable(8) %app, { ptr, ptr } noundef %4)
  %5 = load i32, ptr %argc, align 4
  %6 = load ptr, ptr %argv, align 8
  %7 = call noundef i32 @_ZN14GtkApplication3runEiPPKc(ptr noundef nonnull align 8 dereferenceable(8) %app, i32 noundef %5, ptr noundef %6)
  call void @_ZN14GtkApplication4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %app)
  ret i32 %7
}

declare void @_ZN14GtkApplication4ctorEPKc(ptr, ptr)

declare void @_ZN14GtkApplication19setActivateCallbackEPFv14GtkApplicationPhE(ptr, { ptr, ptr })

declare i32 @_ZN14GtkApplication3runEiPPKc(ptr, i32, ptr)

declare void @_ZN14GtkApplication4dtorEv(ptr noundef nonnull align 8 dereferenceable(8))

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
