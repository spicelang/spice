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

@_ZTS5ITest = dso_local constant [7 x i8] c"5ITest\00", comdat, align 1
@_ZTV8TypeInfo = external global ptr
@_ZTI5ITest = dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5ITest }, comdat, align 8
@_ZTV5ITest = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5ITest, ptr null] }, comdat, align 8
@_ZTS4Test = dso_local constant [6 x i8] c"4Test\00", comdat, align 1
@_ZTI4Test = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS4Test, ptr @_ZTI5ITest }, comdat, align 8
@_ZTV4Test = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI4Test, ptr @_ZN4Test4testEv] }, comdat, align 8
@printf.str.0 = private unnamed_addr constant [5 x i8] c"Test\00", align 1

define private void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV4Test, i32 0, i32 0, i32 2), ptr %2, align 8
  ret void
}

define private void @_ZN4Test4testEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

define private void @_Z7testFctR5ITest(ptr %0) {
  %test = alloca ptr, align 8
  store ptr %0, ptr %test, align 8
  %2 = load ptr, ptr %test, align 8
  %vtable.addr = load ptr, ptr %2, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %2)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %test = alloca %struct.Test, align 8
  %itest = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN4Test4ctorEv(ptr noundef nonnull align 8 dereferenceable(8) %test)
  store ptr %test, ptr %itest, align 8
  %1 = load ptr, ptr %itest, align 8
  call void @_Z7testFctR5ITest(ptr %1)
  ret i32 0
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
