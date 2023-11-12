; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Counter = type { i64 }

@printf.str.0 = private unnamed_addr constant [20 x i8] c"Counter1 value: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Counter2 value: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [20 x i8] c"Counter3 value: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [20 x i8] c"Counter4 value: %d\0A\00", align 1
@printf.str.4 = private unnamed_addr constant [20 x i8] c"Counter5 value: %d\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [20 x i8] c"Counter6 value: %d\0A\00", align 1
@printf.str.6 = private unnamed_addr constant [20 x i8] c"Counter7 value: %d\0A\00", align 1
@printf.str.7 = private unnamed_addr constant [20 x i8] c"Counter8 value: %d\0A\00", align 1
@printf.str.8 = private unnamed_addr constant [20 x i8] c"Counter8 value: %d\0A\00", align 1
@printf.str.9 = private unnamed_addr constant [20 x i8] c"Counter8 value: %d\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [20 x i8] c"Counter8 value: %d\0A\00", align 1
@printf.str.11 = private unnamed_addr constant [20 x i8] c"Counter8 value: %d\0A\00", align 1

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

define private %struct.Counter @_Z8op.minus7Counter7Counter(%struct.Counter %0, %struct.Counter %1) {
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
  %6 = sub i64 %5, %4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %3, i64 %6)
  %7 = load %struct.Counter, ptr %3, align 8
  ret %struct.Counter %7
}

define private %struct.Counter @_Z6op.mul7Counter7Counter(%struct.Counter %0, %struct.Counter %1) {
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
  %6 = mul i64 %5, %4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %3, i64 %6)
  %7 = load %struct.Counter, ptr %3, align 8
  ret %struct.Counter %7
}

define private %struct.Counter @_Z6op.div7Counter7Counter(%struct.Counter %0, %struct.Counter %1) {
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
  %6 = sdiv i64 %5, %4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %3, i64 %6)
  %7 = load %struct.Counter, ptr %3, align 8
  ret %struct.Counter %7
}

define private %struct.Counter @_Z6op.shl7Counter7Counter(%struct.Counter %0, %struct.Counter %1) {
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
  %6 = shl i64 %5, %4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %3, i64 %6)
  %7 = load %struct.Counter, ptr %3, align 8
  ret %struct.Counter %7
}

define private %struct.Counter @_Z6op.shr7Counter7Counter(%struct.Counter %0, %struct.Counter %1) {
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
  %6 = lshr i64 %5, %4
  call void @_ZN7Counter4ctorEl(ptr noundef nonnull align 8 dereferenceable(8) %3, i64 %6)
  %7 = load %struct.Counter, ptr %3, align 8
  ret %struct.Counter %7
}

define private void @_Z12op.plusequalR7Counter7Counter(ptr %0, %struct.Counter %1) {
  %c1 = alloca ptr, align 8
  %c2 = alloca %struct.Counter, align 8
  store ptr %0, ptr %c1, align 8
  store %struct.Counter %1, ptr %c2, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %c2, i32 0, i32 0
  %3 = load ptr, ptr %c1, align 8
  %value_addr1 = getelementptr inbounds %struct.Counter, ptr %3, i32 0, i32 0
  %4 = load i64, ptr %value_addr, align 8
  %5 = load i64, ptr %value_addr1, align 8
  %6 = add i64 %5, %4
  store i64 %6, ptr %value_addr1, align 8
  ret void
}

define private void @_Z13op.minusequalR7Counter7Counter(ptr %0, %struct.Counter %1) {
  %c1 = alloca ptr, align 8
  %c2 = alloca %struct.Counter, align 8
  store ptr %0, ptr %c1, align 8
  store %struct.Counter %1, ptr %c2, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %c2, i32 0, i32 0
  %3 = load ptr, ptr %c1, align 8
  %value_addr1 = getelementptr inbounds %struct.Counter, ptr %3, i32 0, i32 0
  %4 = load i64, ptr %value_addr, align 8
  %5 = load i64, ptr %value_addr1, align 8
  %6 = sub i64 %5, %4
  store i64 %6, ptr %value_addr1, align 8
  ret void
}

define private void @_Z11op.mulequalR7Counter7Counter(ptr %0, %struct.Counter %1) {
  %c1 = alloca ptr, align 8
  %c2 = alloca %struct.Counter, align 8
  store ptr %0, ptr %c1, align 8
  store %struct.Counter %1, ptr %c2, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %c2, i32 0, i32 0
  %3 = load ptr, ptr %c1, align 8
  %value_addr1 = getelementptr inbounds %struct.Counter, ptr %3, i32 0, i32 0
  %4 = load i64, ptr %value_addr, align 8
  %5 = load i64, ptr %value_addr1, align 8
  %6 = mul i64 %5, %4
  store i64 %6, ptr %value_addr1, align 8
  ret void
}

define private void @_Z11op.divequalR7Counter7Counter(ptr %0, %struct.Counter %1) {
  %c1 = alloca ptr, align 8
  %c2 = alloca %struct.Counter, align 8
  store ptr %0, ptr %c1, align 8
  store %struct.Counter %1, ptr %c2, align 8
  %value_addr = getelementptr inbounds %struct.Counter, ptr %c2, i32 0, i32 0
  %3 = load ptr, ptr %c1, align 8
  %value_addr1 = getelementptr inbounds %struct.Counter, ptr %3, i32 0, i32 0
  %4 = load i64, ptr %value_addr, align 8
  %5 = load i64, ptr %value_addr1, align 8
  %6 = sdiv i64 %5, %4
  store i64 %6, ptr %value_addr1, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %counter1 = alloca %struct.Counter, align 8
  %counter2 = alloca %struct.Counter, align 8
  %counter3 = alloca %struct.Counter, align 8
  %counter4 = alloca %struct.Counter, align 8
  %counter5 = alloca %struct.Counter, align 8
  %counter6 = alloca %struct.Counter, align 8
  %counter7 = alloca %struct.Counter, align 8
  %counter8 = alloca %struct.Counter, align 8
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
  %10 = load %struct.Counter, ptr %counter3, align 8
  %11 = load %struct.Counter, ptr %counter2, align 8
  %12 = call %struct.Counter @_Z8op.minus7Counter7Counter(%struct.Counter %10, %struct.Counter %11)
  store %struct.Counter %12, ptr %counter4, align 8
  %13 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter4)
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i64 %13)
  %15 = load %struct.Counter, ptr %counter4, align 8
  %16 = load %struct.Counter, ptr %counter2, align 8
  %17 = call %struct.Counter @_Z6op.mul7Counter7Counter(%struct.Counter %15, %struct.Counter %16)
  store %struct.Counter %17, ptr %counter5, align 8
  %18 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter5)
  %19 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i64 %18)
  %20 = load %struct.Counter, ptr %counter5, align 8
  %21 = load %struct.Counter, ptr %counter2, align 8
  %22 = call %struct.Counter @_Z6op.div7Counter7Counter(%struct.Counter %20, %struct.Counter %21)
  store %struct.Counter %22, ptr %counter6, align 8
  %23 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter6)
  %24 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i64 %23)
  %25 = load %struct.Counter, ptr %counter6, align 8
  %26 = load %struct.Counter, ptr %counter2, align 8
  %27 = call %struct.Counter @_Z6op.shl7Counter7Counter(%struct.Counter %25, %struct.Counter %26)
  store %struct.Counter %27, ptr %counter7, align 8
  %28 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter7)
  %29 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i64 %28)
  %30 = load %struct.Counter, ptr %counter7, align 8
  %31 = load %struct.Counter, ptr %counter2, align 8
  %32 = call %struct.Counter @_Z6op.shr7Counter7Counter(%struct.Counter %30, %struct.Counter %31)
  store %struct.Counter %32, ptr %counter8, align 8
  %33 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter8)
  %34 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i64 %33)
  %35 = load %struct.Counter, ptr %counter2, align 8
  call void @_Z12op.plusequalR7Counter7Counter(ptr %counter8, %struct.Counter %35)
  %36 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter8)
  %37 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i64 %36)
  %38 = load %struct.Counter, ptr %counter2, align 8
  call void @_Z13op.minusequalR7Counter7Counter(ptr %counter8, %struct.Counter %38)
  %39 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter8)
  %40 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, i64 %39)
  %41 = load %struct.Counter, ptr %counter2, align 8
  call void @_Z11op.mulequalR7Counter7Counter(ptr %counter8, %struct.Counter %41)
  %42 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter8)
  %43 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, i64 %42)
  %44 = load %struct.Counter, ptr %counter2, align 8
  call void @_Z11op.divequalR7Counter7Counter(ptr %counter8, %struct.Counter %44)
  %45 = call i64 @_ZN7Counter8getValueEv(ptr noundef nonnull align 8 dereferenceable(8) %counter8)
  %46 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i64 %45)
  %47 = load i32, ptr %result, align 4
  ret i32 %47
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
