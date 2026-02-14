; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.InnerTest = type { %interface.ITest, i32 }
%interface.ITest = type { ptr }
%struct.Test = type { ptr }

@_ZTS5ITest = private constant [7 x i8] c"5ITest\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5ITest = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5ITest }, align 8
@_ZTV5ITest = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5ITest, ptr null] }, align 8
@_ZTS9InnerTest = private constant [11 x i8] c"9InnerTest\00", align 4
@_ZTI9InnerTest = private constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS9InnerTest, ptr @_ZTI5ITest }, align 8
@_ZTV9InnerTest = private unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI9InnerTest, ptr @_ZN9InnerTest4testEv] }, align 8
@printf.str.0 = private unnamed_addr constant [18 x i8] c"InnerTest.test()\0A\00", align 4

define private void @_ZN9InnerTest4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV9InnerTest, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.InnerTest, ptr %2, i32 0, i32 1
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %a.addr = getelementptr inbounds %struct.InnerTest, ptr %4, i64 0, i32 1
  store i32 0, ptr %a.addr, align 4
  ret void
}

define private void @_ZN9InnerTest4testEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %result = alloca i32, align 4
  %innerTest = alloca %struct.InnerTest, align 8
  %test = alloca %struct.Test, align 8
  %test2 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN9InnerTest4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %innerTest)
  store ptr %innerTest, ptr %test, align 8
  store ptr %innerTest, ptr %test2, align 8
  %1 = load ptr, ptr %test2, align 8
  %vtable.addr = load ptr, ptr %1, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %1)
  %2 = load ptr, ptr %test, align 8
  %vtable.addr1 = load ptr, ptr %2, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 0
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  call void %fct3(ptr noundef nonnull align 8 dereferenceable(8) %2)
  %3 = load i32, ptr %result, align 4
  ret i32 %3
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
