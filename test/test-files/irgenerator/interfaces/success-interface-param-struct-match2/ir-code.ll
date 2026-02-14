; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { %interface.ITest }
%interface.ITest = type { ptr }

$_ZTS5ITest = comdat any

$_ZTI5ITest = comdat any

$_ZTV5ITest = comdat any

$_ZTS4Test = comdat any

$_ZTI4Test = comdat any

$_ZTV4Test = comdat any

@_ZTS5ITest = private constant [7 x i8] c"5ITest\00", comdat, align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5ITest = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5ITest }, comdat, align 8
@_ZTV5ITest = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5ITest, ptr null] }, comdat, align 8
@_ZTS4Test = private constant [6 x i8] c"4Test\00", comdat, align 4
@_ZTI4Test = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS4Test, ptr @_ZTI5ITest }, comdat, align 8
@_ZTV4Test = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI4Test, ptr @_ZN4Test3barEv] }, comdat, align 8
@printf.str.0 = private unnamed_addr constant [4 x i8] c"Bar\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV4Test, i64 0, i32 0, i32 2), ptr %2, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 8 dereferenceable(8) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %3, ptr %1, i64 8, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

define private void @_ZN4Test3barEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

define private void @_Z3fooR5ITest(ptr noundef %0) {
  %test = alloca ptr, align 8
  store ptr %0, ptr %test, align 8
  %2 = load ptr, ptr %test, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %2)
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #3 {
  %result = alloca i32, align 4
  %t = alloca %struct.Test, align 8
  %t1 = alloca %struct.Test, align 8
  %tRef = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %t)
  call void @_ZN4Test4ctorERK4Test(ptr noundef nonnull align 8 dereferenceable(8) %t1, ptr %t)
  store ptr %t1, ptr %tRef, align 8
  %1 = load ptr, ptr %tRef, align 8
  call void @_Z3fooR5ITest(ptr noundef %1)
  %2 = load i32, ptr %result, align 4
  ret i32 %2
}

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }
attributes #3 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
