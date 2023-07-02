; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@anon.string.3 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@anon.string.7 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@anon.string.8 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.29 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.30 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.35 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.37 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@anon.string.38 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@printf.str.25 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %6 = alloca ptr, align 8
  %7 = alloca %__String__charptr_long_long, align 8
  %8 = alloca %__String__charptr_long_long, align 8
  %9 = alloca %__String__charptr_long_long, align 8
  %10 = alloca %__String__charptr_long_long, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %__String__charptr_long_long, align 8
  %13 = alloca %__String__charptr_long_long, align 8
  %14 = alloca %__String__charptr_long_long, align 8
  %15 = alloca %__String__charptr_long_long, align 8
  %16 = alloca ptr, align 8
  %17 = alloca %__String__charptr_long_long, align 8
  %18 = alloca %__String__charptr_long_long, align 8
  %19 = alloca %__String__charptr_long_long, align 8
  %20 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %21 = alloca %__String__charptr_long_long, align 8
  %22 = alloca %__String__charptr_long_long, align 8
  %23 = alloca %__String__charptr_long_long, align 8
  %24 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %25 = alloca %__String__charptr_long_long, align 8
  %26 = alloca %__String__charptr_long_long, align 8
  %27 = alloca %__String__charptr_long_long, align 8
  %28 = alloca %__String__charptr_long_long, align 8
  %29 = alloca %__String__charptr_long_long, align 8
  %30 = alloca %__String__charptr_long_long, align 8
  %31 = alloca %__String__charptr_long_long, align 8
  %32 = alloca %__String__charptr_long_long, align 8
  %33 = alloca %__String__charptr_long_long, align 8
  %34 = alloca %__String__charptr_long_long, align 8
  %35 = alloca %__String__charptr_long_long, align 8
  %36 = alloca %__String__charptr_long_long, align 8
  %s4 = alloca %__String__charptr_long_long, align 8
  %37 = alloca i8, align 1
  %s5 = alloca %__String__charptr_long_long, align 8
  %38 = alloca ptr, align 8
  %s6 = alloca %__String__charptr_long_long, align 8
  call void @_mp__String__void__ctor__string(ptr nonnull %1, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %2, ptr nonnull @anon.string.3) #2
  %39 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %1, ptr nonnull %2) #2
  %.fca.0.extract139 = extractvalue %__String__charptr_long_long %39, 0
  store ptr %.fca.0.extract139, ptr %3, align 8
  %.fca.1.extract141 = extractvalue %__String__charptr_long_long %39, 1
  %.fca.1.gep142 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 1
  store i64 %.fca.1.extract141, ptr %.fca.1.gep142, align 8
  %.fca.2.extract143 = extractvalue %__String__charptr_long_long %39, 2
  %.fca.2.gep144 = getelementptr inbounds %__String__charptr_long_long, ptr %3, i64 0, i32 2
  store i64 %.fca.2.extract143, ptr %.fca.2.gep144, align 8
  %40 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %39)
  call void @_mp__String__void__ctor__string(ptr nonnull %4, ptr nonnull @anon.string.10) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %5, ptr nonnull @anon.string.3) #2
  %41 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %4, ptr nonnull %5) #2
  %.fca.0.extract118 = extractvalue %__String__charptr_long_long %41, 0
  %.fca.1.extract120 = extractvalue %__String__charptr_long_long %41, 1
  %.fca.1.gep121 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 1
  %.fca.2.extract122 = extractvalue %__String__charptr_long_long %41, 2
  %.fca.2.gep123 = getelementptr inbounds %__String__charptr_long_long, ptr %s1, i64 0, i32 2
  store ptr %.fca.0.extract118, ptr %s1, align 8
  store i64 %.fca.1.extract120, ptr %.fca.1.gep121, align 8
  store i64 %.fca.2.extract122, ptr %.fca.2.gep123, align 8
  %42 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %41)
  store ptr @anon.string.4, ptr %6, align 8
  %43 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr nonnull %s1, ptr nonnull %6) #2
  %.fca.0.extract112 = extractvalue %__String__charptr_long_long %43, 0
  store ptr %.fca.0.extract112, ptr %7, align 8
  %.fca.1.extract114 = extractvalue %__String__charptr_long_long %43, 1
  %.fca.1.gep115 = getelementptr inbounds %__String__charptr_long_long, ptr %7, i64 0, i32 1
  store i64 %.fca.1.extract114, ptr %.fca.1.gep115, align 8
  %.fca.2.extract116 = extractvalue %__String__charptr_long_long %43, 2
  %.fca.2.gep117 = getelementptr inbounds %__String__charptr_long_long, ptr %7, i64 0, i32 2
  store i64 %.fca.2.extract116, ptr %.fca.2.gep117, align 8
  %44 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %43)
  call void @_mp__String__void__ctor__string(ptr nonnull %8, ptr nonnull @anon.string.5) #2
  %45 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %8, ptr nonnull %s1) #2
  %.fca.0.extract106 = extractvalue %__String__charptr_long_long %45, 0
  store ptr %.fca.0.extract106, ptr %9, align 8
  %.fca.1.extract108 = extractvalue %__String__charptr_long_long %45, 1
  %.fca.1.gep109 = getelementptr inbounds %__String__charptr_long_long, ptr %9, i64 0, i32 1
  store i64 %.fca.1.extract108, ptr %.fca.1.gep109, align 8
  %.fca.2.extract110 = extractvalue %__String__charptr_long_long %45, 2
  %.fca.2.gep111 = getelementptr inbounds %__String__charptr_long_long, ptr %9, i64 0, i32 2
  store i64 %.fca.2.extract110, ptr %.fca.2.gep111, align 8
  %46 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %45)
  %47 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %s1, ptr nonnull %s1) #2
  %.fca.0.extract100 = extractvalue %__String__charptr_long_long %47, 0
  store ptr %.fca.0.extract100, ptr %10, align 8
  %.fca.1.extract102 = extractvalue %__String__charptr_long_long %47, 1
  %.fca.1.gep103 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 1
  store i64 %.fca.1.extract102, ptr %.fca.1.gep103, align 8
  %.fca.2.extract104 = extractvalue %__String__charptr_long_long %47, 2
  %.fca.2.gep105 = getelementptr inbounds %__String__charptr_long_long, ptr %10, i64 0, i32 2
  store i64 %.fca.2.extract104, ptr %.fca.2.gep105, align 8
  %48 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %47)
  store ptr @anon.string.6, ptr %11, align 8
  %49 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr nonnull %s1, ptr nonnull %11) #2
  %.fca.0.extract94 = extractvalue %__String__charptr_long_long %49, 0
  store ptr %.fca.0.extract94, ptr %12, align 8
  %.fca.1.extract96 = extractvalue %__String__charptr_long_long %49, 1
  %.fca.1.gep97 = getelementptr inbounds %__String__charptr_long_long, ptr %12, i64 0, i32 1
  store i64 %.fca.1.extract96, ptr %.fca.1.gep97, align 8
  %.fca.2.extract98 = extractvalue %__String__charptr_long_long %49, 2
  %.fca.2.gep99 = getelementptr inbounds %__String__charptr_long_long, ptr %12, i64 0, i32 2
  store i64 %.fca.2.extract98, ptr %.fca.2.gep99, align 8
  %50 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %12, ptr nonnull %s1) #2
  %.fca.0.extract88 = extractvalue %__String__charptr_long_long %50, 0
  store ptr %.fca.0.extract88, ptr %13, align 8
  %.fca.1.extract90 = extractvalue %__String__charptr_long_long %50, 1
  %.fca.1.gep91 = getelementptr inbounds %__String__charptr_long_long, ptr %13, i64 0, i32 1
  store i64 %.fca.1.extract90, ptr %.fca.1.gep91, align 8
  %.fca.2.extract92 = extractvalue %__String__charptr_long_long %50, 2
  %.fca.2.gep93 = getelementptr inbounds %__String__charptr_long_long, ptr %13, i64 0, i32 2
  store i64 %.fca.2.extract92, ptr %.fca.2.gep93, align 8
  %51 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %50)
  call void @_mp__String__void__ctor__string(ptr nonnull %14, ptr nonnull @anon.string.7) #2
  %52 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr nonnull %14, ptr nonnull %s1) #2
  %.fca.0.extract82 = extractvalue %__String__charptr_long_long %52, 0
  store ptr %.fca.0.extract82, ptr %15, align 8
  %.fca.1.extract84 = extractvalue %__String__charptr_long_long %52, 1
  %.fca.1.gep85 = getelementptr inbounds %__String__charptr_long_long, ptr %15, i64 0, i32 1
  store i64 %.fca.1.extract84, ptr %.fca.1.gep85, align 8
  %.fca.2.extract86 = extractvalue %__String__charptr_long_long %52, 2
  %.fca.2.gep87 = getelementptr inbounds %__String__charptr_long_long, ptr %15, i64 0, i32 2
  store i64 %.fca.2.extract86, ptr %.fca.2.gep87, align 8
  store ptr @anon.string.8, ptr %16, align 8
  %53 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr nonnull %15, ptr nonnull %16) #2
  %.fca.0.extract76 = extractvalue %__String__charptr_long_long %53, 0
  store ptr %.fca.0.extract76, ptr %17, align 8
  %.fca.1.extract78 = extractvalue %__String__charptr_long_long %53, 1
  %.fca.1.gep79 = getelementptr inbounds %__String__charptr_long_long, ptr %17, i64 0, i32 1
  store i64 %.fca.1.extract78, ptr %.fca.1.gep79, align 8
  %.fca.2.extract80 = extractvalue %__String__charptr_long_long %53, 2
  %.fca.2.gep81 = getelementptr inbounds %__String__charptr_long_long, ptr %17, i64 0, i32 2
  store i64 %.fca.2.extract80, ptr %.fca.2.gep81, align 8
  %54 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %53)
  call void @_mp__String__void__ctor__string(ptr nonnull %18, ptr nonnull @anon.string.38) #2
  %55 = call %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16 4, ptr nonnull %18) #2
  %.fca.0.extract70 = extractvalue %__String__charptr_long_long %55, 0
  store ptr %.fca.0.extract70, ptr %19, align 8
  %.fca.1.extract72 = extractvalue %__String__charptr_long_long %55, 1
  %.fca.1.gep73 = getelementptr inbounds %__String__charptr_long_long, ptr %19, i64 0, i32 1
  store i64 %.fca.1.extract72, ptr %.fca.1.gep73, align 8
  %.fca.2.extract74 = extractvalue %__String__charptr_long_long %55, 2
  %.fca.2.gep75 = getelementptr inbounds %__String__charptr_long_long, ptr %19, i64 0, i32 2
  store i64 %.fca.2.extract74, ptr %.fca.2.gep75, align 8
  %56 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %55)
  call void @_mp__String__void__ctor__string(ptr nonnull %20, ptr nonnull @anon.string.10) #2
  %57 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr nonnull %20, i32 5) #2
  %.fca.0.extract49 = extractvalue %__String__charptr_long_long %57, 0
  %.fca.1.extract51 = extractvalue %__String__charptr_long_long %57, 1
  %.fca.1.gep52 = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 1
  %.fca.2.extract53 = extractvalue %__String__charptr_long_long %57, 2
  %.fca.2.gep54 = getelementptr inbounds %__String__charptr_long_long, ptr %s2, i64 0, i32 2
  store ptr %.fca.0.extract49, ptr %s2, align 8
  store i64 %.fca.1.extract51, ptr %.fca.1.gep52, align 8
  store i64 %.fca.2.extract53, ptr %.fca.2.gep54, align 8
  %58 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %57)
  call void @_mp__String__void__ctor__char(ptr nonnull %21, i8 97) #2
  %59 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 20, ptr nonnull %21) #2
  %.fca.0.extract43 = extractvalue %__String__charptr_long_long %59, 0
  store ptr %.fca.0.extract43, ptr %22, align 8
  %.fca.1.extract45 = extractvalue %__String__charptr_long_long %59, 1
  %.fca.1.gep46 = getelementptr inbounds %__String__charptr_long_long, ptr %22, i64 0, i32 1
  store i64 %.fca.1.extract45, ptr %.fca.1.gep46, align 8
  %.fca.2.extract47 = extractvalue %__String__charptr_long_long %59, 2
  %.fca.2.gep48 = getelementptr inbounds %__String__charptr_long_long, ptr %22, i64 0, i32 2
  store i64 %.fca.2.extract47, ptr %.fca.2.gep48, align 8
  %60 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %59)
  call void @_mp__String__void__ctor__char(ptr nonnull %23, i8 99) #2
  %61 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 2, ptr nonnull %23) #2
  %.fca.0.extract37 = extractvalue %__String__charptr_long_long %61, 0
  store ptr %.fca.0.extract37, ptr %24, align 8
  %.fca.1.extract39 = extractvalue %__String__charptr_long_long %61, 1
  %.fca.1.gep40 = getelementptr inbounds %__String__charptr_long_long, ptr %24, i64 0, i32 1
  store i64 %.fca.1.extract39, ptr %.fca.1.gep40, align 8
  %.fca.2.extract41 = extractvalue %__String__charptr_long_long %61, 2
  %.fca.2.gep42 = getelementptr inbounds %__String__charptr_long_long, ptr %24, i64 0, i32 2
  store i64 %.fca.2.extract41, ptr %.fca.2.gep42, align 8
  %62 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr nonnull %24, i32 7) #2
  %.fca.0.extract = extractvalue %__String__charptr_long_long %62, 0
  %.fca.1.extract = extractvalue %__String__charptr_long_long %62, 1
  %.fca.1.gep20 = getelementptr inbounds %__String__charptr_long_long, ptr %s3, i64 0, i32 1
  %.fca.2.extract = extractvalue %__String__charptr_long_long %62, 2
  %.fca.2.gep21 = getelementptr inbounds %__String__charptr_long_long, ptr %s3, i64 0, i32 2
  store ptr %.fca.0.extract, ptr %s3, align 8
  store i64 %.fca.1.extract, ptr %.fca.1.gep20, align 8
  store i64 %.fca.2.extract, ptr %.fca.2.gep21, align 8
  %63 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %62)
  %64 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %65 = zext i1 %64 to i32
  %66 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %65)
  %67 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.32) #2
  %68 = zext i1 %67 to i32
  %69 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %68)
  %70 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.35) #2
  %71 = zext i1 %70 to i32
  %72 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.13, i32 %71)
  call void @_mp__String__void__ctor__string(ptr nonnull %25, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %26, ptr nonnull @anon.string.30) #2
  %73 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr nonnull %25, ptr nonnull %26) #2
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %74)
  call void @_mp__String__void__ctor__string(ptr nonnull %27, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %28, ptr nonnull @anon.string.32) #2
  %76 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr nonnull %27, ptr nonnull %28) #2
  %77 = zext i1 %76 to i32
  %78 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %77)
  call void @_mp__String__void__ctor__string(ptr nonnull %29, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %30, ptr nonnull @anon.string.35) #2
  %79 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr nonnull %29, ptr nonnull %30) #2
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.16, i32 %80)
  %82 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.29, ptr nonnull @anon.string.30) #2
  %83 = xor i1 %82, true
  %84 = zext i1 %83 to i32
  %85 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %84)
  %86 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.32) #2
  %87 = xor i1 %86, true
  %88 = zext i1 %87 to i32
  %89 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %88)
  %90 = call i1 @_f__void__bool__isRawEqual__string_string(ptr nonnull @anon.string.35, ptr nonnull @anon.string.35) #2
  %91 = xor i1 %90, true
  %92 = zext i1 %91 to i32
  %93 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.19, i32 %92)
  call void @_mp__String__void__ctor__string(ptr nonnull %31, ptr nonnull @anon.string.29) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %32, ptr nonnull @anon.string.30) #2
  %94 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr nonnull %31, ptr nonnull %32) #2
  %95 = zext i1 %94 to i32
  %96 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %95)
  call void @_mp__String__void__ctor__string(ptr nonnull %33, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %34, ptr nonnull @anon.string.32) #2
  %97 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr nonnull %33, ptr nonnull %34) #2
  %98 = zext i1 %97 to i32
  %99 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %98)
  call void @_mp__String__void__ctor__string(ptr nonnull %35, ptr nonnull @anon.string.35) #2
  call void @_mp__String__void__ctor__string(ptr nonnull %36, ptr nonnull @anon.string.35) #2
  %100 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr nonnull %35, ptr nonnull %36) #2
  %101 = zext i1 %100 to i32
  %102 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.22, i32 %101)
  call void @_mp__String__void__ctor__string(ptr nonnull %s4, ptr nonnull @anon.string.35) #2
  store i8 108, ptr %37, align 1
  call void @_p__void__void__op.plusequal__Stringref_charref(ptr nonnull %s4, ptr nonnull %37) #2
  %.fca.0.load11 = load ptr, ptr %s4, align 8
  %.fca.0.insert12 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load11, 0
  %.fca.1.gep13 = getelementptr inbounds %__String__charptr_long_long, ptr %s4, i64 0, i32 1
  %.fca.1.load14 = load i64, ptr %.fca.1.gep13, align 8
  %.fca.1.insert15 = insertvalue %__String__charptr_long_long %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.gep16 = getelementptr inbounds %__String__charptr_long_long, ptr %s4, i64 0, i32 2
  %.fca.2.load17 = load i64, ptr %.fca.2.gep16, align 8
  %.fca.2.insert18 = insertvalue %__String__charptr_long_long %.fca.1.insert15, i64 %.fca.2.load17, 2
  %103 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %.fca.2.insert18)
  call void @_mp__String__void__ctor__string(ptr nonnull %s5, ptr nonnull @anon.string.38) #2
  store ptr @anon.string.37, ptr %38, align 8
  call void @_p__void__void__op.plusequal__Stringref_stringref(ptr nonnull %s5, ptr nonnull %38) #2
  %.fca.0.load2 = load ptr, ptr %s5, align 8
  %.fca.0.insert3 = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %__String__charptr_long_long, ptr %s5, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %__String__charptr_long_long %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %__String__charptr_long_long, ptr %s5, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %__String__charptr_long_long %.fca.1.insert6, i64 %.fca.2.load8, 2
  %104 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %.fca.2.insert9)
  call void @_mp__String__void__ctor__string(ptr nonnull %s6, ptr nonnull @anon.string.38) #2
  call void @_p__void__void__op.mulequal__Stringref_int(ptr nonnull %s6, i32 3) #2
  %.fca.0.load = load ptr, ptr %s6, align 8
  %.fca.0.insert = insertvalue %__String__charptr_long_long poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s6, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %__String__charptr_long_long %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %__String__charptr_long_long, ptr %s6, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %__String__charptr_long_long %.fca.1.insert, i64 %.fca.2.load, 2
  %105 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.25, %__String__charptr_long_long %.fca.2.insert)
  call void @_mp__String__void__dtor(ptr nonnull %36) #2
  call void @_mp__String__void__dtor(ptr nonnull %33) #2
  call void @_mp__String__void__dtor(ptr nonnull %31) #2
  call void @_mp__String__void__dtor(ptr nonnull %32) #2
  call void @_mp__String__void__dtor(ptr nonnull %10) #2
  call void @_mp__String__void__dtor(ptr nonnull %15) #2
  call void @_mp__String__void__dtor(ptr nonnull %24) #2
  call void @_mp__String__void__dtor(ptr nonnull %26) #2
  call void @_mp__String__void__dtor(ptr nonnull %12) #2
  call void @_mp__String__void__dtor(ptr nonnull %7) #2
  call void @_mp__String__void__dtor(ptr nonnull %28) #2
  call void @_mp__String__void__dtor(ptr nonnull %5) #2
  call void @_mp__String__void__dtor(ptr nonnull %35) #2
  call void @_mp__String__void__dtor(ptr nonnull %2) #2
  call void @_mp__String__void__dtor(ptr nonnull %s6) #2
  call void @_mp__String__void__dtor(ptr nonnull %s4) #2
  call void @_mp__String__void__dtor(ptr nonnull %17) #2
  call void @_mp__String__void__dtor(ptr nonnull %s5) #2
  call void @_mp__String__void__dtor(ptr nonnull %3) #2
  call void @_mp__String__void__dtor(ptr nonnull %34) #2
  call void @_mp__String__void__dtor(ptr nonnull %s3) #2
  call void @_mp__String__void__dtor(ptr nonnull %s1) #2
  call void @_mp__String__void__dtor(ptr nonnull %13) #2
  call void @_mp__String__void__dtor(ptr nonnull %s2) #2
  call void @_mp__String__void__dtor(ptr nonnull %18) #2
  call void @_mp__String__void__dtor(ptr nonnull %19) #2
  call void @_mp__String__void__dtor(ptr nonnull %23) #2
  call void @_mp__String__void__dtor(ptr nonnull %21) #2
  call void @_mp__String__void__dtor(ptr nonnull %22) #2
  call void @_mp__String__void__dtor(ptr nonnull %25) #2
  call void @_mp__String__void__dtor(ptr nonnull %27) #2
  call void @_mp__String__void__dtor(ptr nonnull %9) #2
  call void @_mp__String__void__dtor(ptr nonnull %30) #2
  call void @_mp__String__void__dtor(ptr nonnull %29) #2
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_mp__String__void__ctor__string(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16, ptr) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr, i32) local_unnamed_addr

declare void @_mp__String__void__ctor__char(ptr, i8) local_unnamed_addr

declare %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32, ptr) local_unnamed_addr

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr) local_unnamed_addr

declare i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr, ptr) local_unnamed_addr

declare i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__op.plusequal__Stringref_charref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__op.plusequal__Stringref_stringref(ptr, ptr) local_unnamed_addr

declare void @_p__void__void__op.mulequal__Stringref_int(ptr, i32) local_unnamed_addr

declare void @_mp__String__void__dtor(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
