; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@2 = private unnamed_addr constant [15 x i8] c"Hello World!?!\00", align 1
@3 = private unnamed_addr constant [12 x i8] c"Equals: %d\0A\00", align 1
@4 = private unnamed_addr constant [14 x i8] c"Hello World!!\00", align 1
@5 = private unnamed_addr constant [13 x i8] c"Content: %s\0A\00", align 1
@6 = private unnamed_addr constant [12 x i8] c"Length: %d\0A\00", align 1
@7 = private unnamed_addr constant [15 x i8] c"Capacity: %d\0A\0A\00", align 1
@8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@9 = private unnamed_addr constant [11 x i8] c"Empty: %d\0A\00", align 1
@10 = private unnamed_addr constant [10 x i8] c"Empty: %d\00", align 1

define i32 @main() local_unnamed_addr {
entry.l1:
  %s = alloca %_s__String__charptr_long_long, align 8
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %s, ptr nonnull @0)
  %2 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s)
  %3 = call i32 (ptr, ...) @printf(ptr nonnull @5, ptr %2)
  %4 = call i64 @_mf__String__long__getLength(ptr nonnull %s)
  %5 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %4)
  %6 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s)
  %7 = call i32 (ptr, ...) @printf(ptr nonnull @7, i64 %6)
  call void @_mp__String__void__append__string(ptr nonnull %s, ptr nonnull @1)
  %8 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s)
  %9 = call i32 (ptr, ...) @printf(ptr nonnull @5, ptr %8)
  %10 = call i64 @_mf__String__long__getLength(ptr nonnull %s)
  %11 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %10)
  %12 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s)
  %13 = call i32 (ptr, ...) @printf(ptr nonnull @7, i64 %12)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 63)
  %14 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s)
  %15 = call i32 (ptr, ...) @printf(ptr nonnull @5, ptr %14)
  %16 = call i64 @_mf__String__long__getLength(ptr nonnull %s)
  %17 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %16)
  %18 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s)
  %19 = call i32 (ptr, ...) @printf(ptr nonnull @7, i64 %18)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 33)
  %20 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s)
  %21 = call i32 (ptr, ...) @printf(ptr nonnull @5, ptr %20)
  %22 = call i64 @_mf__String__long__getLength(ptr nonnull %s)
  %23 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %22)
  %24 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s)
  %25 = call i32 (ptr, ...) @printf(ptr nonnull @7, i64 %24)
  call void @_mp__String__void__ctor__string(ptr nonnull %0, ptr nonnull @2)
  %.fca.0.load16 = load ptr, ptr %0, align 8
  %.fca.0.insert17 = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load16, 0
  %.fca.1.gep18 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 1
  %.fca.1.load19 = load i64, ptr %.fca.1.gep18, align 8
  %.fca.1.insert20 = insertvalue %_s__String__charptr_long_long %.fca.0.insert17, i64 %.fca.1.load19, 1
  %.fca.2.gep21 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 2
  %.fca.2.load22 = load i64, ptr %.fca.2.gep21, align 8
  %.fca.2.insert23 = insertvalue %_s__String__charptr_long_long %.fca.1.insert20, i64 %.fca.2.load22, 2
  %26 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %s, %_s__String__charptr_long_long %.fca.2.insert23)
  %27 = zext i1 %26 to i32
  %28 = call i32 (ptr, ...) @printf(ptr nonnull @3, i32 %27)
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @4)
  %.fca.0.load = load ptr, ptr %1, align 8
  %.fca.0.insert = insertvalue %_s__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %_s__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %_s__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %29 = call i1 @_mf__String__bool__isEqual__String(ptr nonnull %s, %_s__String__charptr_long_long %.fca.2.insert)
  %30 = zext i1 %29 to i32
  %31 = call i32 (ptr, ...) @printf(ptr nonnull @3, i32 %30)
  call void @_mp__String__void__clear(ptr nonnull %s)
  %32 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s)
  %33 = call i32 (ptr, ...) @printf(ptr nonnull @5, ptr %32)
  %34 = call i64 @_mf__String__long__getLength(ptr nonnull %s)
  %35 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %34)
  %36 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s)
  %37 = call i32 (ptr, ...) @printf(ptr nonnull @7, i64 %36)
  call void @_mp__String__void__reserve__long(ptr nonnull %s, i64 100)
  %38 = call ptr @_mf__String__charptr__getRaw(ptr nonnull %s)
  %39 = call i32 (ptr, ...) @printf(ptr nonnull @5, ptr %38)
  %40 = call i64 @_mf__String__long__getLength(ptr nonnull %s)
  %41 = call i32 (ptr, ...) @printf(ptr nonnull @6, i64 %40)
  %42 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s)
  %43 = call i32 (ptr, ...) @printf(ptr nonnull @7, i64 %42)
  call void @_mp__String__void__dtor(ptr nonnull %s)
  call void @_mp__String__void__ctor__string(ptr nonnull %s, ptr nonnull @8)
  %44 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s)
  %45 = zext i1 %44 to i32
  %46 = call i32 (ptr, ...) @printf(ptr nonnull @9, i32 %45)
  call void @_mp__String__void__append__char(ptr nonnull %s, i8 97)
  %47 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s)
  %48 = zext i1 %47 to i32
  %49 = call i32 (ptr, ...) @printf(ptr nonnull @10, i32 %48)
  call void @_mp__String__void__dtor(ptr nonnull %0)
  call void @_mp__String__void__dtor(ptr nonnull %1)
  call void @_mp__String__void__dtor(ptr nonnull %s)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare ptr @_mf__String__charptr__getRaw(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getCapacity(ptr) local_unnamed_addr

declare void @_mp__String__void__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__void__append__char(ptr, i8) local_unnamed_addr

declare i1 @_mf__String__bool__isEqual__String(ptr, %_s__String__charptr_long_long) local_unnamed_addr

declare void @_mp__String__void__clear(ptr) local_unnamed_addr

declare void @_mp__String__void__reserve__long(ptr, i64) local_unnamed_addr

declare void @_mp__String__void__dtor(ptr) local_unnamed_addr

declare i1 @_mf__String__bool__isEmpty(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
