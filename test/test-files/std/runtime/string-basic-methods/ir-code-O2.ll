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
  %1 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %s4 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.0) #2
  %.fca.0.load41 = load ptr, ptr %1, align 8
  %.fca.1.gep43 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 1
  %.fca.1.load44 = load i64, ptr %.fca.1.gep43, align 8
  %.fca.2.gep46 = getelementptr inbounds %__String__charptr_long_long, ptr %1, i64 0, i32 2
  %.fca.2.load47 = load i64, ptr %.fca.2.gep46, align 8
  store ptr %.fca.0.load41, ptr %s1, align 8
  %.fca.1.gep37 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 1
  store i64 %.fca.1.load44, ptr %.fca.1.gep37, align 8
  %.fca.2.gep39 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 2
  store i64 %.fca.2.load47, ptr %.fca.2.gep39, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.10) #2
  %.fca.0.load26 = load ptr, ptr %2, align 8
  %.fca.1.gep28 = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 1
  %.fca.1.load29 = load i64, ptr %.fca.1.gep28, align 8
  %.fca.2.gep31 = getelementptr inbounds %__String__charptr_long_long, ptr %2, i64 0, i32 2
  %.fca.2.load32 = load i64, ptr %.fca.2.gep31, align 8
  store ptr %.fca.0.load26, ptr %s2, align 8
  %.fca.1.gep22 = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 1
  store i64 %.fca.1.load29, ptr %.fca.1.gep22, align 8
  %.fca.2.gep24 = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 2
  store i64 %.fca.2.load32, ptr %.fca.2.gep24, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %3, ptr nonnull @anon.string.2) #2
  %.fca.0.load11 = load ptr, ptr %3, align 8
  %.fca.1.gep13 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 1
  %.fca.1.load14 = load i64, ptr %.fca.1.gep13, align 8
  %.fca.2.gep16 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 2
  %.fca.2.load17 = load i64, ptr %.fca.2.gep16, align 8
  store ptr %.fca.0.load11, ptr %s3, align 8
  %.fca.1.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %s3, i64 0, i32 1
  store i64 %.fca.1.load14, ptr %.fca.1.gep7, align 8
  %.fca.2.gep9 = getelementptr inbounds %__String__charptr_long_long, ptr %s3, i64 0, i32 2
  store i64 %.fca.2.load17, ptr %.fca.2.gep9, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @anon.string.3) #2
  %.fca.0.load = load ptr, ptr %4, align 8
  %.fca.1.gep2 = getelementptr inbounds %__String__charptr_long_long, ptr %4, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep2, align 8
  %.fca.2.gep3 = getelementptr inbounds %__String__charptr_long_long, ptr %4, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep3, align 8
  store ptr %.fca.0.load, ptr %s4, align 8
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s4, i64 0, i32 1
  store i64 %.fca.1.load, ptr %.fca.1.gep, align 8
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s4, i64 0, i32 2
  store i64 %.fca.2.load, ptr %.fca.2.gep, align 8
  %5 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s1) #2
  %6 = zext i1 %5 to i32
  %7 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %6)
  %8 = call i1 @_mf__String__bool__isEmpty(ptr nonnull %s2) #2
  %9 = zext i1 %8 to i32
  %10 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %9)
  %11 = call i64 @_mf__String__long__getLength(ptr nonnull %s3) #2
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %11)
  %13 = call i64 @_mf__String__long__getLength(ptr nonnull %s4) #2
  %14 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %13)
  %15 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s3) #2
  %16 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %15)
  %17 = call i64 @_mf__String__long__getCapacity(ptr nonnull %s4) #2
  %18 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %17)
  %19 = call i1 @_mf__String__bool__isFull(ptr nonnull %s2) #2
  %20 = zext i1 %19 to i32
  %21 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %20)
  %22 = call i1 @_mf__String__bool__isFull(ptr nonnull %s4) #2
  %23 = zext i1 %22 to i32
  %24 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %23)
  %25 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.4) #2
  %26 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %25)
  %27 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.5) #2
  %28 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %27)
  %29 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.6) #2
  %30 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %29)
  %31 = call i64 @_mf__String__long__find__string(ptr nonnull %s4, ptr nonnull @anon.string.7) #2
  %32 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %31)
  %33 = call i64 @_mf__String__long__find__string_int(ptr nonnull %s4, ptr nonnull @anon.string.8, i32 12) #2
  %34 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i64 %33)
  %35 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.9) #2
  %36 = zext i1 %35 to i32
  %37 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %36)
  %38 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.10) #2
  %39 = zext i1 %38 to i32
  %40 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %39)
  %41 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.11) #2
  %42 = zext i1 %41 to i32
  %43 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %42)
  %44 = call i1 @_mf__String__bool__contains__string(ptr nonnull %s4, ptr nonnull @anon.string.12) #2
  %45 = zext i1 %44 to i32
  %46 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %45)
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
