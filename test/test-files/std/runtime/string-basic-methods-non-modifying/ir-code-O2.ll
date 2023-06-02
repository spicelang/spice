; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Hello!\00", align 1
@anon.string.3 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.4 = private unnamed_addr constant [4 x i8] c"ell\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Wort\00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c"H\00", align 1
@anon.string.7 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@anon.string.8 = private unnamed_addr constant [2 x i8] c" \00", align 1
@anon.string.9 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@anon.string.10 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.11 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@printf.str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@anon.string.12 = private unnamed_addr constant [4 x i8] c"o W\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %s1 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %s4 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %s1, ptr nonnull @anon.string.0) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %s2, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %s3, ptr nonnull @anon.string.2) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %s4, ptr nonnull @anon.string.3) #2
  %1 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s1) #2
  %2 = zext i1 %1 to i32
  %3 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %2)
  %4 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s2) #2
  %5 = zext i1 %4 to i32
  %6 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %5)
  %7 = call i64 @_mf__String__long__getLength(ptr nonnull %s3) #2
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %7)
  %9 = call i64 @_mf__String__long__getLength(ptr nonnull %s4) #2
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %9)
  %11 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s3) #2
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %11)
  %13 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s4) #2
  %14 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %13)
  %15 = call i1 @_mf__String__bool__isFull(ptr nonnull %s2) #2
  %16 = zext i1 %15 to i32
  %17 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %16)
  %18 = call i1 @_mf__String__bool__isFull(ptr nonnull %s4) #2
  %19 = zext i1 %18 to i32
  %20 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %19)
  %21 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.4) #2
  %22 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %21)
  %23 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.5) #2
  %24 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %23)
  %25 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.6) #2
  %26 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %25)
  %27 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.7) #2
  %28 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %27)
  %29 = call i64 @_mf__String__long__find__string_int(ptr nonnull %s4, ptr nonnull @anon.string.8, i32 12) #2
  %30 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %29)
  %31 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.9) #2
  %32 = zext i1 %31 to i32
  %33 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %32)
  %34 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.10) #2
  %35 = zext i1 %34 to i32
  %36 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %35)
  %37 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.11) #2
  %38 = zext i1 %37 to i32
  %39 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %38)
  %40 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.12) #2
  %41 = zext i1 %40 to i32
  %42 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %41)
  ret i32 0
}

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare i1 @_mf__String__bool__isEmpty(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getLength(ptr) local_unnamed_addr

declare i64 @_mf__String__long__getCapacity(ptr) local_unnamed_addr

declare i1 @_mf__String__bool__isFull(ptr) local_unnamed_addr

declare i64 @_mf__String__long__find__string(ptr, ptr) local_unnamed_addr

declare i64 @_mf__String__long__find__string_int(ptr, ptr, i32) local_unnamed_addr

declare i1 @_mf__String__bool__contains__string(ptr, ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
