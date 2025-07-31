; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { i1, ptr }

@anon.string.0 = private unnamed_addr constant [12 x i8] c"Test string\00", align 4
@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@1 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@anon.string.1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 4
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [16 x i8] c"Another message\00", align 4
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Fields: %d, %s\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Message: %s\0A\00", align 4

define private void @_ZN6Vector4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr @anon.string.0, ptr %msg, align 8
  %2 = load ptr, ptr %this, align 8
  store i1 false, ptr %2, align 1
  %3 = getelementptr inbounds nuw %struct.Vector, ptr %2, i32 0, i32 1
  store ptr @0, ptr %3, align 8
  %4 = load ptr, ptr %this, align 8
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %4, i64 0, i32 0
  store i1 false, ptr %field1.addr, align 1
  %5 = load ptr, ptr %this, align 8
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %5, i64 0, i32 1
  %6 = load ptr, ptr %msg, align 8
  store ptr %6, ptr %field2.addr, align 8
  ret void
}

define private void @_ZN6Vector4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(16) %0, ptr %1) {
  %this = alloca ptr, align 8
  %msg = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  store ptr %1, ptr %msg, align 8
  %3 = load ptr, ptr %this, align 8
  store i1 false, ptr %3, align 1
  %4 = getelementptr inbounds nuw %struct.Vector, ptr %3, i32 0, i32 1
  store ptr @1, ptr %4, align 8
  %5 = load ptr, ptr %this, align 8
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %5, i64 0, i32 0
  store i1 false, ptr %field1.addr, align 1
  %6 = load ptr, ptr %this, align 8
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %6, i64 0, i32 1
  %7 = load ptr, ptr %msg, align 8
  store ptr %7, ptr %field2.addr, align 8
  ret void
}

define private ptr @_ZN6Vector4testEv(ptr noundef nonnull align 8 dereferenceable(16) %0) {
  %result = alloca ptr, align 8
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  ret ptr @anon.string.1
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %vec = alloca %struct.Vector, align 8
  %1 = alloca %struct.Vector, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN6Vector4ctorEv(ptr noundef nonnull align 8 dereferenceable(16) %vec)
  %field1.addr = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 0
  %2 = load i1, ptr %field1.addr, align 1
  %3 = zext i1 %2 to i32
  %field2.addr = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 1
  %4 = load ptr, ptr %field2.addr, align 8
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3, ptr %4)
  call void @_ZN6Vector4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(16) %1, ptr @anon.string.2)
  %6 = load %struct.Vector, ptr %1, align 8
  store %struct.Vector %6, ptr %vec, align 8
  %field1.addr1 = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 0
  %7 = load i1, ptr %field1.addr1, align 1
  %8 = zext i1 %7 to i32
  %field2.addr2 = getelementptr inbounds %struct.Vector, ptr %vec, i64 0, i32 1
  %9 = load ptr, ptr %field2.addr2, align 8
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %8, ptr %9)
  %11 = call ptr @_ZN6Vector4testEv(ptr noundef nonnull align 8 dereferenceable(16) %vec)
  %12 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %11)
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
