; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Counter = type { i64 }

@printf.str.0 = private unnamed_addr constant [20 x i8] c"Counter1 value: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Counter2 value: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [20 x i8] c"Counter3 value: %d\0A\00", align 1

define private void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %0, i64 %1) {
  %this = alloca ptr, align 8
  %initialValue = alloca i64, align 8
  store ptr %0, ptr %this, align 8
  store i64 %1, ptr %initialValue, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = getelementptr inbounds %struct.Counter, ptr %3, i32 0, i32 0
  store i64 0, ptr %4, align 8
  %5 = load ptr, ptr %this, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %5, i32 0, i32 0
  %6 = load i64, ptr %initialValue, align 8
  store i64 %6, ptr %value_addr, align 8
  ret void
}

define private i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %0) {
  %result = alloca i64, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %2, i32 0, i32 0
  %3 = load i64, ptr %value_addr, align 8
  ret i64 %3
}

define private %struct.Counter @_Z7op.plus7Counter7Counter(%struct.Counter %0, %struct.Counter %1) {
  %result = alloca %struct.Counter, align 8
  %c1 = alloca %struct.Counter, align 8
  %c2 = alloca %struct.Counter, align 8
  %3 = alloca %struct.Counter, align 8
  store %struct.Counter %0, ptr %c1, align 8
  store %struct.Counter %1, ptr %c2, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %c1, i32 0, i32 0
  %value_addr1 = getelementptr inbounds %struct.Counter, ptr %c2, i32 0, i32 0
  %4 = load i64, ptr %value_addr1, align 8
  %5 = load i64, ptr %value_addr, align 8
  %6 = add i64 %5, %4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %3, i64 %6)
  %7 = load %struct.Counter, ptr %3, align 8
  ret %struct.Counter %7
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %counter1 = alloca %struct.Counter, align 8
  %counter2 = alloca %struct.Counter, align 8
  %counter3 = alloca %struct.Counter, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %counter1, i64 2)
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %counter2, i64 3)
  %1 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter1)
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 %1)
  %3 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter2)
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 %3)
  %5 = load %struct.Counter, ptr %counter1, align 8
  %6 = load %struct.Counter, ptr %counter2, align 8
  %7 = call %struct.Counter @_Z7op.plus7Counter7Counter(%struct.Counter %5, %struct.Counter %6)
  store %struct.Counter %7, ptr %counter3, align 8
  %8 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter3)
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
