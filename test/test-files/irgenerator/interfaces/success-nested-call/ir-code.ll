; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.InnerTest = type { %interface.ITest, i32 }
%interface.ITest = type { ptr }
%struct.Test = type { ptr }

$_ZTS5ITest = comdat any

$_ZTI5ITest = comdat any

$_ZTV5ITest = comdat any

$_ZTS9InnerTest = comdat any

$_ZTI9InnerTest = comdat any

$_ZTV9InnerTest = comdat any

@_ZTS5ITest = dso_local constant [7 x i8] c"5ITest\00", comdat, align 1
@_ZTV8TypeInfo = external global ptr
@_ZTI5ITest = dso_local constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5ITest }, comdat, align 8
@_ZTV5ITest = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI5ITest, ptr null] }, comdat, align 8
@_ZTS9InnerTest = dso_local constant [11 x i8] c"9InnerTest\00", comdat, align 1
@_ZTI9InnerTest = dso_local constant { ptr, ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS9InnerTest, ptr @_ZTI5ITest }, comdat, align 8
@_ZTV9InnerTest = dso_local unnamed_addr constant { [3 x ptr] } { [3 x ptr] [ptr null, ptr @_ZTI9InnerTest, ptr @_ZN9InnerTest4testEv] }, comdat, align 8
@printf.str.0 = private unnamed_addr constant [18 x i8] c"InnerTest.test()\0A\00", align 1

define private void @_ZN9InnerTest4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [3 x ptr] }, ptr @_ZTV9InnerTest, i32 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds %struct.InnerTest, ptr %2, i32 0, i32 1
  store i32 0, ptr %3, align 4
  %4 = load ptr, ptr %this, align 8
  %a_addr = getelementptr inbounds %struct.InnerTest, ptr %4, i32 0, i32 1
  store i32 0, ptr %a_addr, align 4
  ret void
}

define private void @_ZN9InnerTest4testEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %innerTest = alloca %struct.InnerTest, align 8
  %test = alloca %struct.Test, align 8
  %test2 = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN9InnerTest4ctorEv(ptr %innerTest)
  store ptr %innerTest, ptr %test, align 8
  store ptr %innerTest, ptr %test2, align 8
  %1 = load ptr, ptr %test2, align 8
  %vtable.addr = load ptr, ptr %1, align 8
  %vfct.addr = getelementptr inbounds ptr, ptr %vtable.addr, i64 0
  %fct = load ptr, ptr %vfct.addr, align 8
  call void %fct(ptr noundef nonnull align 8 dereferenceable(8) %1)
  %2 = getelementptr inbounds %struct.Test, ptr %test, i32 0, i32 0
  %3 = load ptr, ptr %2, align 8
  %vtable.addr1 = load ptr, ptr %3, align 8
  %vfct.addr2 = getelementptr inbounds ptr, ptr %vtable.addr1, i64 0
  %fct3 = load ptr, ptr %vfct.addr2, align 8
  call void %fct3(ptr noundef nonnull align 8 dereferenceable(8) %3)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
