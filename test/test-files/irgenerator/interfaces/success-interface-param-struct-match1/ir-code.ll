; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Test = type { %interface.ITest }
%interface.ITest = type { ptr }

@_ZTS5ITest = private constant [7 x i8] c"5ITest\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5ITest = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5ITest }, align 8
@_ZTV5ITest = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5ITest, ptr null] }, align 8
@_ZTS4Test = private constant [6 x i8] c"4Test\00", align 4
@_ZTI4Test = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS4Test, ptr @_ZTI5ITest }, align 8
@_ZTV4Test = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI4Test, ptr @_ZN4Test4testEv] }, align 8
@printf.str.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 4

define private void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV4Test, i64 0, i32 0, i32 2), ptr %2, align 8
  ret void
}

define private void @_ZN4Test4testEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

define private void @_Z7testFctR5ITest(ptr noundef %0) {
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
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %test = alloca %struct.Test, align 8
  %itest = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %test)
  store ptr %test, ptr %itest, align 8
  %1 = load ptr, ptr %itest, align 8
  call void @_Z7testFctR5ITest(ptr noundef %1)
  ret i32 0
}

attributes #0 = { nofree nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
