; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__String__charptr_long_long = type { ptr, i64, i64 }

@0 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@1 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@2 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@4 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@5 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@6 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@7 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@8 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@9 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

define i32 @main() local_unnamed_addr {
entry.l3:
  %0 = alloca %_s__String__charptr_long_long, align 8
  %1 = alloca %_s__String__charptr_long_long, align 8
  %2 = alloca %_s__String__charptr_long_long, align 8
  %3 = alloca %_s__String__charptr_long_long, align 8
  %4 = alloca %_s__String__charptr_long_long, align 8
  %5 = alloca %_s__String__charptr_long_long, align 8
  %6 = alloca %_s__String__charptr_long_long, align 8
  %7 = alloca %_s__String__charptr_long_long, align 8
  %8 = alloca %_s__String__charptr_long_long, align 8
  %9 = alloca %_s__String__charptr_long_long, align 8
  %10 = alloca %_s__String__charptr_long_long, align 8
  call void @_mp__String__ctor__string_string(ptr nonnull %0, ptr nonnull @1, ptr nonnull @0)
  %.fca.0.load64 = load ptr, ptr %0, align 8
  %.fca.1.gep66 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 1
  %.fca.1.load67 = load i64, ptr %.fca.1.gep66, align 8
  %.fca.2.gep69 = getelementptr inbounds %_s__String__charptr_long_long, ptr %0, i64 0, i32 2
  %.fca.2.load70 = load i64, ptr %.fca.2.gep69, align 8
  store ptr %.fca.0.load64, ptr %1, align 8
  %.fca.1.gep60 = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 1
  store i64 %.fca.1.load67, ptr %.fca.1.gep60, align 8
  %.fca.2.gep62 = getelementptr inbounds %_s__String__charptr_long_long, ptr %1, i64 0, i32 2
  store i64 %.fca.2.load70, ptr %.fca.2.gep62, align 8
  %11 = call ptr @_mf__String__getRaw(ptr nonnull %1)
  %12 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %11)
  call void @_mp__String__ctor__string_string(ptr nonnull %2, ptr nonnull @1, ptr nonnull @0)
  %.fca.0.load49 = load ptr, ptr %2, align 8
  %.fca.1.gep51 = getelementptr inbounds %_s__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load52 = load i64, ptr %.fca.1.gep51, align 8
  %.fca.2.gep54 = getelementptr inbounds %_s__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load55 = load i64, ptr %.fca.2.gep54, align 8
  store ptr %.fca.0.load49, ptr %3, align 8
  %.fca.1.gep46 = getelementptr inbounds %_s__String__charptr_long_long, ptr %3, i64 0, i32 1
  store i64 %.fca.1.load52, ptr %.fca.1.gep46, align 8
  %.fca.2.gep47 = getelementptr inbounds %_s__String__charptr_long_long, ptr %3, i64 0, i32 2
  store i64 %.fca.2.load55, ptr %.fca.2.gep47, align 8
  %13 = call ptr @_mf__String__getRaw(ptr nonnull %3)
  %14 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %13)
  call void @_mp__String__ctor__string(ptr nonnull %4, ptr nonnull @9)
  call void @_mp__String__opMul__short(ptr nonnull %4, i16 4)
  %15 = call ptr @_mf__String__getRaw(ptr nonnull %4)
  %16 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %15)
  call void @_mp__String__ctor__string(ptr nonnull %5, ptr nonnull @1)
  call void @_mp__String__opMul__int(ptr nonnull %5, i32 5)
  %17 = call ptr @_mf__String__getRaw(ptr nonnull %5)
  %18 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %17)
  call void @_mp__String__ctor__char(ptr nonnull %6, i8 97)
  call void @_mp__String__opMul__int(ptr nonnull %6, i32 20)
  %19 = call ptr @_mf__String__getRaw(ptr nonnull %6)
  %20 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %19)
  call void @_mp__String__ctor__char(ptr nonnull %7, i8 99)
  call void @_mp__String__opMul__int(ptr nonnull %7, i32 2)
  call void @_mp__String__opMul__int(ptr nonnull %7, i32 7)
  %21 = call ptr @_mf__String__getRaw(ptr nonnull %7)
  %22 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %21)
  %23 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 zext (i1 icmp eq (ptr @3, ptr @4) to i32))
  %24 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 zext (i1 icmp eq (ptr @7, ptr @5) to i32))
  %25 = call i32 (ptr, ...) @printf(ptr nonnull @2, i32 1)
  %26 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 zext (i1 icmp ne (ptr @3, ptr @4) to i32))
  %27 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 zext (i1 icmp ne (ptr @7, ptr @5) to i32))
  %28 = call i32 (ptr, ...) @printf(ptr nonnull @6, i32 0)
  call void @_mp__String__ctor__string(ptr nonnull %8, ptr nonnull @7)
  call void @_mp__String__append__char(ptr nonnull %8, i8 108)
  %29 = call ptr @_mf__String__getRaw(ptr nonnull %8)
  %30 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %29)
  call void @_mp__String__ctor__string(ptr nonnull %9, ptr nonnull @9)
  call void @_mp__String__append__string(ptr nonnull %9, ptr nonnull @8)
  %31 = call ptr @_mf__String__getRaw(ptr nonnull %9)
  %32 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %31)
  call void @_mp__String__ctor__string(ptr nonnull %10, ptr nonnull @9)
  call void @_mp__String__opMul__int(ptr nonnull %10, i32 3)
  %33 = call ptr @_mf__String__getRaw(ptr nonnull %10)
  %34 = call i32 (ptr, ...) @printf(ptr nonnull @10, ptr %33)
  call void @_mp__String__dtor(ptr nonnull %5)
  call void @_mp__String__dtor(ptr nonnull %6)
  call void @_mp__String__dtor(ptr nonnull %7)
  call void @_mp__String__dtor(ptr nonnull %8)
  call void @_mp__String__dtor(ptr nonnull %9)
  call void @_mp__String__dtor(ptr nonnull %10)
  call void @_mp__String__dtor(ptr nonnull %0)
  call void @_mp__String__dtor(ptr nonnull %2)
  call void @_mp__String__dtor(ptr nonnull %4)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

declare void @_mp__String__ctor__string_string(ptr, ptr, ptr) local_unnamed_addr

declare ptr @_mf__String__getRaw(ptr) local_unnamed_addr

declare void @_mp__String__ctor__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__opMul__short(ptr, i16) local_unnamed_addr

declare void @_mp__String__opMul__int(ptr, i32) local_unnamed_addr

declare void @_mp__String__ctor__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__append__char(ptr, i8) local_unnamed_addr

declare void @_mp__String__append__string(ptr, ptr) local_unnamed_addr

declare void @_mp__String__dtor(ptr) local_unnamed_addr

attributes #0 = { nofree nounwind }
