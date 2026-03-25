; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.GtkWidget = type { ptr }
%struct.GtkWindow = type { %struct.GtkWidget }
%struct.GtkApplication = type { ptr }
%struct.Result = type { ptr, %struct.Error }
%struct.Error = type { i32, ptr }
%struct.String = type { ptr, i64, i64 }
%struct.GtkBuilder = type { ptr }
%struct.Result.0 = type { i1, %struct.Error }
%struct.GtkButton = type { %struct.GtkWidget }

@anon.string.0 = private unnamed_addr constant [14 x i8] c"Hello World!\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [14 x i8] c"SPICE_STD_DIR\00", align 4
@anon.string.2 = private unnamed_addr constant [61 x i8] c"/../test/test-files/std/bindings/gtk4-with-builder/window.ui\00", align 4
@anon.string.3 = private unnamed_addr constant [7 x i8] c"window\00", align 4
@anon.string.4 = private unnamed_addr constant [8 x i8] c"button1\00", align 4
@anon.string.5 = private unnamed_addr constant [8 x i8] c"button2\00", align 4
@anon.string.6 = private unnamed_addr constant [5 x i8] c"quit\00", align 4
@anon.string.7 = private unnamed_addr constant [22 x i8] c"com.spicelang.Example\00", align 4

define private void @_Z8btnClick9GtkWidget(%struct.GtkWidget noundef %0) {
  %widget = alloca %struct.GtkWidget, align 8
  store %struct.GtkWidget %0, ptr %widget, align 8
  call void @_Z6gPrintPKc(ptr noundef @anon.string.0)
  ret void
}

declare void @_Z6gPrintPKc(ptr)

define private void @_Z4quit9GtkWidget9GtkWindow(%struct.GtkWidget noundef %0, %struct.GtkWindow noundef %1) {
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
  %spiceStdDir = alloca %struct.Result, align 8
  %filePathString = alloca %struct.String, align 8
  %3 = alloca ptr, align 8
  %builder = alloca %struct.GtkBuilder, align 8
  %result = alloca %struct.Result.0, align 8
  %window = alloca %struct.GtkWindow, align 8
  %button1 = alloca %struct.GtkButton, align 8
  %fat.ptr = alloca { ptr, ptr }, align 8
  %button2 = alloca %struct.GtkButton, align 8
  %fat.ptr1 = alloca { ptr, ptr }, align 8
  %quitButton = alloca %struct.GtkButton, align 8
  %fat.ptr2 = alloca { ptr, ptr }, align 8
  store %struct.GtkApplication %0, ptr %app, align 8
  store ptr %1, ptr %data, align 8
  %4 = call noundef %struct.Result @_Z6getEnvPKc(ptr noundef @anon.string.1)
  store %struct.Result %4, ptr %spiceStdDir, align 8
  %5 = call noundef ptr @_ZN6ResultIPKcE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %spiceStdDir)
  %6 = load ptr, ptr %5, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %filePathString, ptr noundef %6)
  store ptr @anon.string.2, ptr %3, align 8
  call void @_Z12op.plusequalIPKcEvR6StringRKPKc(ptr %filePathString, ptr %3)
  call void @_ZN10GtkBuilder4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %builder)
  %7 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %filePathString)
  %8 = call noundef %struct.Result.0 @_ZN10GtkBuilder11addFromFileEPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef %7)
  store %struct.Result.0 %8, ptr %result, align 8
  %9 = call noundef ptr @_ZN6ResultIbE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %result)
  %10 = call noundef %struct.GtkWindow @_ZN10GtkBuilder9getObjectI9GtkWindowEE9GtkWindowPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.3)
  store %struct.GtkWindow %10, ptr %window, align 8
  %11 = load %struct.GtkApplication, ptr %app, align 8
  call void @_ZN9GtkWindow14setApplicationE14GtkApplication(ptr noundef nonnull align 8 dereferenceable(8) %window, %struct.GtkApplication noundef %11)
  %12 = call noundef %struct.GtkButton @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.4)
  store %struct.GtkButton %12, ptr %button1, align 8
  store ptr @_Z8btnClick9GtkWidget, ptr %fat.ptr, align 8
  %13 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr, i32 0, i32 1
  store ptr poison, ptr %13, align 8
  %14 = load { ptr, ptr }, ptr %fat.ptr, align 8
  call void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr noundef nonnull align 8 dereferenceable(8) %button1, { ptr, ptr } noundef %14)
  %15 = call noundef %struct.GtkButton @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.5)
  store %struct.GtkButton %15, ptr %button2, align 8
  store ptr @_Z8btnClick9GtkWidget, ptr %fat.ptr1, align 8
  %16 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr1, i32 0, i32 1
  store ptr poison, ptr %16, align 8
  %17 = load { ptr, ptr }, ptr %fat.ptr1, align 8
  call void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr noundef nonnull align 8 dereferenceable(8) %button2, { ptr, ptr } noundef %17)
  %18 = call noundef %struct.GtkButton @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.6)
  store %struct.GtkButton %18, ptr %quitButton, align 8
  store ptr @_Z4quit9GtkWidget9GtkWindow, ptr %fat.ptr2, align 8
  %19 = getelementptr inbounds nuw { ptr, ptr }, ptr %fat.ptr2, i32 0, i32 1
  store ptr poison, ptr %19, align 8
  %20 = load { ptr, ptr }, ptr %fat.ptr2, align 8
  %21 = load %struct.GtkWindow, ptr %window, align 8
  call void @_ZN9GtkButton18setOnClickCallbackI9GtkWindowEEvPFv9GtkWidget9GtkWindowE9GtkWindow(ptr noundef nonnull align 8 dereferenceable(8) %quitButton, { ptr, ptr } noundef %20, %struct.GtkWindow noundef %21)
  call void @_ZN9GtkWindow10setVisibleEv(ptr noundef nonnull align 8 dereferenceable(8) %window)
  call void @_ZN10GtkBuilder4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %builder)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %filePathString)
  ret void
}

declare %struct.Result @_Z6getEnvPKc(ptr)

declare ptr @_ZN6ResultIPKcE6unwrapEv(ptr)

declare void @_ZN6String4ctorEPKc(ptr, ptr)

declare void @_Z12op.plusequalIPKcEvR6StringRKPKc(ptr, ptr)

declare void @_ZN10GtkBuilder4ctorEv(ptr noundef nonnull align 8 dereferenceable(8))

declare ptr @_ZN6String6getRawEv(ptr)

declare %struct.Result.0 @_ZN10GtkBuilder11addFromFileEPKc(ptr, ptr)

declare ptr @_ZN6ResultIbE6unwrapEv(ptr)

declare %struct.GtkWindow @_ZN10GtkBuilder9getObjectI9GtkWindowEE9GtkWindowPKc(ptr, ptr)

declare void @_ZN9GtkWindow14setApplicationE14GtkApplication(ptr, %struct.GtkApplication)

declare %struct.GtkButton @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr, ptr)

declare void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr, { ptr, ptr })

declare void @_ZN9GtkButton18setOnClickCallbackI9GtkWindowEEvPFv9GtkWidget9GtkWindowE9GtkWindow(ptr, { ptr, ptr }, %struct.GtkWindow)

declare void @_ZN9GtkWindow10setVisibleEv(ptr)

declare void @_ZN10GtkBuilder4dtorEv(ptr noundef nonnull align 8 dereferenceable(8))

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

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
  call void @_ZN14GtkApplication4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(8) %app, ptr noundef @anon.string.7)
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
