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

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z8btnClick9GtkWidget(%struct.GtkWidget noundef %0) #0 {
  %widget = alloca %struct.GtkWidget, align 8
  store %struct.GtkWidget %0, ptr %widget, align 8
  call void @_Z6gPrintPKc(ptr noundef @anon.string.0)
  ret void
}

declare void @_Z6gPrintPKc(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z4quit9GtkWidget9GtkWindow(%struct.GtkWidget noundef %0, %struct.GtkWindow noundef %1) #0 {
  %widget = alloca %struct.GtkWidget, align 8
  %window = alloca %struct.GtkWindow, align 8
  store %struct.GtkWidget %0, ptr %widget, align 8
  store %struct.GtkWindow %1, ptr %window, align 8
  call void @_ZN9GtkWindow7destroyEv(ptr noundef nonnull align 8 dereferenceable(8) %window)
  ret void
}

declare void @_ZN9GtkWindow7destroyEv(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z8activate14GtkApplicationPh(%struct.GtkApplication noundef %0, ptr noundef align 1 %1) #0 {
  %app = alloca %struct.GtkApplication, align 8
  %data = alloca ptr, align 8
  %spiceStdDir = alloca %struct.Result, align 8
  %filePathString = alloca %struct.String, align 8
  %3 = alloca ptr, align 8
  %builder = alloca %struct.GtkBuilder, align 8
  %result = alloca %struct.Result.0, align 8
  %window = alloca %struct.GtkWindow, align 8
  %button1 = alloca %struct.GtkButton, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %button2 = alloca %struct.GtkButton, align 8
  %fat.ptr1 = alloca { ptr, ptr, i64 }, align 8
  %quitButton = alloca %struct.GtkButton, align 8
  %fat.ptr2 = alloca { ptr, ptr, i64 }, align 8
  store %struct.GtkApplication %0, ptr %app, align 8
  store ptr %1, ptr %data, align 8
  call void @_Z6getEnvPKc(ptr dead_on_unwind writable sret(%struct.Result) align 8 %spiceStdDir, ptr noundef @anon.string.1)
  %4 = call noundef ptr @_ZN6ResultIPKcE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %spiceStdDir)
  %5 = load ptr, ptr %4, align 8
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %filePathString, ptr noundef %5)
  store ptr @anon.string.2, ptr %3, align 8
  call void @_Z12op.plusequalIPKcEvR6StringRKPKc(ptr %filePathString, ptr %3)
  call void @_ZN10GtkBuilder4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %builder)
  %6 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %filePathString)
  call void @_ZN10GtkBuilder11addFromFileEPKc(ptr dead_on_unwind writable sret(%struct.Result.0) align 8 %result, ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef %6)
  %7 = call noundef ptr @_ZN6ResultIbE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %result)
  %8 = call ptr @_ZN10GtkBuilder9getObjectI9GtkWindowEE9GtkWindowPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.3)
  store ptr %8, ptr %window, align 8
  %9 = load %struct.GtkApplication, ptr %app, align 8
  call void @_ZN9GtkWindow14setApplicationE14GtkApplication(ptr noundef nonnull align 8 dereferenceable(8) %window, %struct.GtkApplication noundef %9)
  %10 = call ptr @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.4)
  store ptr %10, ptr %button1, align 8
  store ptr @_Z8btnClick9GtkWidget.fatthunk, ptr %fat.ptr, align 8
  %11 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1
  store ptr null, ptr %11, align 8
  %12 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2
  store i64 0, ptr %12, align 8
  %13 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8
  call void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr noundef nonnull align 8 dereferenceable(8) %button1, { ptr, ptr, i64 } noundef %13)
  %14 = call ptr @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.5)
  store ptr %14, ptr %button2, align 8
  store ptr @_Z8btnClick9GtkWidget.fatthunk, ptr %fat.ptr1, align 8
  %15 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 1
  store ptr null, ptr %15, align 8
  %16 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr1, i32 0, i32 2
  store i64 0, ptr %16, align 8
  %17 = load { ptr, ptr, i64 }, ptr %fat.ptr1, align 8
  call void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr noundef nonnull align 8 dereferenceable(8) %button2, { ptr, ptr, i64 } noundef %17)
  %18 = call ptr @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr noundef nonnull align 8 dereferenceable(8) %builder, ptr noundef @anon.string.6)
  store ptr %18, ptr %quitButton, align 8
  store ptr @_Z4quit9GtkWidget9GtkWindow.fatthunk, ptr %fat.ptr2, align 8
  %19 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 1
  store ptr null, ptr %19, align 8
  %20 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 2
  store i64 0, ptr %20, align 8
  %21 = load { ptr, ptr, i64 }, ptr %fat.ptr2, align 8
  %22 = load %struct.GtkWindow, ptr %window, align 8
  call void @_ZN9GtkButton18setOnClickCallbackI9GtkWindowEEvPFv9GtkWidget9GtkWindowE9GtkWindow(ptr noundef nonnull align 8 dereferenceable(8) %quitButton, { ptr, ptr, i64 } noundef %21, %struct.GtkWindow noundef %22)
  call void @_ZN9GtkWindow10setVisibleEv(ptr noundef nonnull align 8 dereferenceable(8) %window)
  call void @_ZN10GtkBuilder4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %builder)
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %filePathString)
  ret void
}

declare void @_Z6getEnvPKc(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8, ptr)

declare ptr @_ZN6ResultIPKcE6unwrapEv(ptr)

declare void @_ZN6String4ctorEPKc(ptr, ptr)

declare void @_Z12op.plusequalIPKcEvR6StringRKPKc(ptr, ptr)

declare void @_ZN10GtkBuilder4ctorEv(ptr noundef nonnull align 8 dereferenceable(8))

declare ptr @_ZN6String6getRawEv(ptr)

declare void @_ZN10GtkBuilder11addFromFileEPKc(ptr dead_on_unwind noalias writable sret(%struct.Result.0) align 8, ptr, ptr)

declare ptr @_ZN6ResultIbE6unwrapEv(ptr)

declare ptr @_ZN10GtkBuilder9getObjectI9GtkWindowEE9GtkWindowPKc(ptr, ptr)

declare void @_ZN9GtkWindow14setApplicationE14GtkApplication(ptr, %struct.GtkApplication)

declare ptr @_ZN10GtkBuilder9getObjectI9GtkButtonEE9GtkButtonPKc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define private void @_Z8btnClick9GtkWidget.fatthunk(%struct.GtkWidget %0, ptr %1) #0 {
entry:
  call void @_Z8btnClick9GtkWidget(%struct.GtkWidget %0)
  ret void
}

declare void @_ZN9GtkButton18setOnClickCallbackEPFv9GtkWidgetE(ptr, { ptr, ptr, i64 })

; Function Attrs: noinline nounwind optnone uwtable
define private void @_Z4quit9GtkWidget9GtkWindow.fatthunk(%struct.GtkWidget %0, %struct.GtkWindow %1, ptr %2) #0 {
entry:
  call void @_Z4quit9GtkWidget9GtkWindow(%struct.GtkWidget %0, %struct.GtkWindow %1)
  ret void
}

declare void @_ZN9GtkButton18setOnClickCallbackI9GtkWindowEEvPFv9GtkWidget9GtkWindowE9GtkWindow(ptr, { ptr, ptr, i64 }, %struct.GtkWindow)

declare void @_ZN9GtkWindow10setVisibleEv(ptr)

declare void @_ZN10GtkBuilder4dtorEv(ptr noundef nonnull align 8 dereferenceable(8))

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main(i32 %0, ptr %1) #1 {
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
  %app = alloca %struct.GtkApplication, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  store i32 %0, ptr %argc, align 4
  store ptr %1, ptr %argv, align 8
  call void @_ZN14GtkApplication4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(8) %app, ptr noundef @anon.string.7)
  store ptr @_Z8activate14GtkApplicationPh.fatthunk, ptr %fat.ptr, align 8
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1
  store ptr null, ptr %3, align 8
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2
  store i64 0, ptr %4, align 8
  %5 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8
  call void @_ZN14GtkApplication19setActivateCallbackEPFv14GtkApplicationPhE(ptr noundef nonnull align 8 dereferenceable(8) %app, { ptr, ptr, i64 } noundef %5)
  %6 = load i32, ptr %argc, align 4
  %7 = load ptr, ptr %argv, align 8
  %8 = call noundef i32 @_ZN14GtkApplication3runEiPPKc(ptr noundef nonnull align 8 dereferenceable(8) %app, i32 noundef %6, ptr noundef %7)
  call void @_ZN14GtkApplication4dtorEv(ptr noundef nonnull align 8 dereferenceable(8) %app)
  ret i32 %8
}

declare void @_ZN14GtkApplication4ctorEPKc(ptr, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define private void @_Z8activate14GtkApplicationPh.fatthunk(%struct.GtkApplication %0, ptr %1, ptr %2) #0 {
entry:
  call void @_Z8activate14GtkApplicationPh(%struct.GtkApplication %0, ptr %1)
  ret void
}

declare void @_ZN14GtkApplication19setActivateCallbackEPFv14GtkApplicationPhE(ptr, { ptr, ptr, i64 })

declare i32 @_ZN14GtkApplication3runEiPPKc(ptr, i32, ptr)

declare void @_ZN14GtkApplication4dtorEv(ptr noundef nonnull align 8 dereferenceable(8))

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
