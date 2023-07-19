; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

@anon.string.3 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@anon.string.7 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@anon.string.8 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.11 = private unnamed_addr constant [4 x i8] c"One\00", align 1
@printf.str.14 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@printf.str.17 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@printf.str.20 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.30 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.31 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@anon.string.33 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.23 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.36 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.38 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@anon.string.39 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@printf.str.26 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca %struct.String, align 8
  %2 = alloca %struct.String, align 8
  %3 = alloca %struct.String, align 8
  %4 = alloca %struct.String, align 8
  %5 = alloca %struct.String, align 8
  %s1 = alloca %struct.String, align 8
  %6 = alloca ptr, align 8
  %7 = alloca %struct.String, align 8
  %8 = alloca %struct.String, align 8
  %9 = alloca %struct.String, align 8
  %10 = alloca %struct.String, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %struct.String, align 8
  %13 = alloca %struct.String, align 8
  %14 = alloca %struct.String, align 8
  %15 = alloca %struct.String, align 8
  %16 = alloca ptr, align 8
  %17 = alloca %struct.String, align 8
  %18 = alloca %struct.String, align 8
  %19 = alloca %struct.String, align 8
  %20 = alloca %struct.String, align 8
  %s2 = alloca %struct.String, align 8
  %21 = alloca %struct.String, align 8
  %22 = alloca %struct.String, align 8
  %23 = alloca %struct.String, align 8
  %24 = alloca %struct.String, align 8
  %s3 = alloca %struct.String, align 8
  %25 = alloca %struct.String, align 8
  %26 = alloca %struct.String, align 8
  %27 = alloca %struct.String, align 8
  %28 = alloca %struct.String, align 8
  %29 = alloca %struct.String, align 8
  %30 = alloca %struct.String, align 8
  %31 = alloca %struct.String, align 8
  %32 = alloca %struct.String, align 8
  %33 = alloca %struct.String, align 8
  %34 = alloca %struct.String, align 8
  %35 = alloca %struct.String, align 8
  %36 = alloca %struct.String, align 8
  %37 = alloca %struct.String, align 8
  %38 = alloca %struct.String, align 8
  %s4 = alloca %struct.String, align 8
  %39 = alloca i8, align 1
  %s5 = alloca %struct.String, align 8
  %40 = alloca ptr, align 8
  %s6 = alloca %struct.String, align 8
  call void @_ZN6String4ctorEPc(ptr nonnull %1, ptr nonnull @anon.string.10) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %2, ptr nonnull @anon.string.3) #2
  %41 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr nonnull %1, ptr nonnull %2) #2
  %.fca.0.extract145 = extractvalue %struct.String %41, 0
  store ptr %.fca.0.extract145, ptr %3, align 8
  %.fca.1.extract147 = extractvalue %struct.String %41, 1
  %.fca.1.gep148 = getelementptr inbounds %struct.String, ptr %3, i64 0, i32 1
  store i64 %.fca.1.extract147, ptr %.fca.1.gep148, align 8
  %.fca.2.extract149 = extractvalue %struct.String %41, 2
  %.fca.2.gep150 = getelementptr inbounds %struct.String, ptr %3, i64 0, i32 2
  store i64 %.fca.2.extract149, ptr %.fca.2.gep150, align 8
  %42 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %41)
  call void @_ZN6String4ctorEPc(ptr nonnull %4, ptr nonnull @anon.string.10) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %5, ptr nonnull @anon.string.3) #2
  %43 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr nonnull %4, ptr nonnull %5) #2
  %.fca.0.extract124 = extractvalue %struct.String %43, 0
  %.fca.1.extract126 = extractvalue %struct.String %43, 1
  %.fca.1.gep127 = getelementptr inbounds %struct.String, ptr %s1, i64 0, i32 1
  %.fca.2.extract128 = extractvalue %struct.String %43, 2
  %.fca.2.gep129 = getelementptr inbounds %struct.String, ptr %s1, i64 0, i32 2
  store ptr %.fca.0.extract124, ptr %s1, align 8
  store i64 %.fca.1.extract126, ptr %.fca.1.gep127, align 8
  store i64 %.fca.2.extract128, ptr %.fca.2.gep129, align 8
  %44 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %43)
  store ptr @anon.string.4, ptr %6, align 8
  %45 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr nonnull %s1, ptr nonnull %6) #2
  %.fca.0.extract118 = extractvalue %struct.String %45, 0
  store ptr %.fca.0.extract118, ptr %7, align 8
  %.fca.1.extract120 = extractvalue %struct.String %45, 1
  %.fca.1.gep121 = getelementptr inbounds %struct.String, ptr %7, i64 0, i32 1
  store i64 %.fca.1.extract120, ptr %.fca.1.gep121, align 8
  %.fca.2.extract122 = extractvalue %struct.String %45, 2
  %.fca.2.gep123 = getelementptr inbounds %struct.String, ptr %7, i64 0, i32 2
  store i64 %.fca.2.extract122, ptr %.fca.2.gep123, align 8
  %46 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %45)
  call void @_ZN6String4ctorEPc(ptr nonnull %8, ptr nonnull @anon.string.5) #2
  %47 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr nonnull %8, ptr nonnull %s1) #2
  %.fca.0.extract112 = extractvalue %struct.String %47, 0
  store ptr %.fca.0.extract112, ptr %9, align 8
  %.fca.1.extract114 = extractvalue %struct.String %47, 1
  %.fca.1.gep115 = getelementptr inbounds %struct.String, ptr %9, i64 0, i32 1
  store i64 %.fca.1.extract114, ptr %.fca.1.gep115, align 8
  %.fca.2.extract116 = extractvalue %struct.String %47, 2
  %.fca.2.gep117 = getelementptr inbounds %struct.String, ptr %9, i64 0, i32 2
  store i64 %.fca.2.extract116, ptr %.fca.2.gep117, align 8
  %48 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %47)
  %49 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr nonnull %s1, ptr nonnull %s1) #2
  %.fca.0.extract106 = extractvalue %struct.String %49, 0
  store ptr %.fca.0.extract106, ptr %10, align 8
  %.fca.1.extract108 = extractvalue %struct.String %49, 1
  %.fca.1.gep109 = getelementptr inbounds %struct.String, ptr %10, i64 0, i32 1
  store i64 %.fca.1.extract108, ptr %.fca.1.gep109, align 8
  %.fca.2.extract110 = extractvalue %struct.String %49, 2
  %.fca.2.gep111 = getelementptr inbounds %struct.String, ptr %10, i64 0, i32 2
  store i64 %.fca.2.extract110, ptr %.fca.2.gep111, align 8
  %50 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %49)
  store ptr @anon.string.6, ptr %11, align 8
  %51 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr nonnull %s1, ptr nonnull %11) #2
  %.fca.0.extract100 = extractvalue %struct.String %51, 0
  store ptr %.fca.0.extract100, ptr %12, align 8
  %.fca.1.extract102 = extractvalue %struct.String %51, 1
  %.fca.1.gep103 = getelementptr inbounds %struct.String, ptr %12, i64 0, i32 1
  store i64 %.fca.1.extract102, ptr %.fca.1.gep103, align 8
  %.fca.2.extract104 = extractvalue %struct.String %51, 2
  %.fca.2.gep105 = getelementptr inbounds %struct.String, ptr %12, i64 0, i32 2
  store i64 %.fca.2.extract104, ptr %.fca.2.gep105, align 8
  %52 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr nonnull %12, ptr nonnull %s1) #2
  %.fca.0.extract94 = extractvalue %struct.String %52, 0
  store ptr %.fca.0.extract94, ptr %13, align 8
  %.fca.1.extract96 = extractvalue %struct.String %52, 1
  %.fca.1.gep97 = getelementptr inbounds %struct.String, ptr %13, i64 0, i32 1
  store i64 %.fca.1.extract96, ptr %.fca.1.gep97, align 8
  %.fca.2.extract98 = extractvalue %struct.String %52, 2
  %.fca.2.gep99 = getelementptr inbounds %struct.String, ptr %13, i64 0, i32 2
  store i64 %.fca.2.extract98, ptr %.fca.2.gep99, align 8
  %53 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %52)
  call void @_ZN6String4ctorEPc(ptr nonnull %14, ptr nonnull @anon.string.7) #2
  %54 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr nonnull %14, ptr nonnull %s1) #2
  %.fca.0.extract88 = extractvalue %struct.String %54, 0
  store ptr %.fca.0.extract88, ptr %15, align 8
  %.fca.1.extract90 = extractvalue %struct.String %54, 1
  %.fca.1.gep91 = getelementptr inbounds %struct.String, ptr %15, i64 0, i32 1
  store i64 %.fca.1.extract90, ptr %.fca.1.gep91, align 8
  %.fca.2.extract92 = extractvalue %struct.String %54, 2
  %.fca.2.gep93 = getelementptr inbounds %struct.String, ptr %15, i64 0, i32 2
  store i64 %.fca.2.extract92, ptr %.fca.2.gep93, align 8
  store ptr @anon.string.8, ptr %16, align 8
  %55 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr nonnull %15, ptr nonnull %16) #2
  %.fca.0.extract82 = extractvalue %struct.String %55, 0
  store ptr %.fca.0.extract82, ptr %17, align 8
  %.fca.1.extract84 = extractvalue %struct.String %55, 1
  %.fca.1.gep85 = getelementptr inbounds %struct.String, ptr %17, i64 0, i32 1
  store i64 %.fca.1.extract84, ptr %.fca.1.gep85, align 8
  %.fca.2.extract86 = extractvalue %struct.String %55, 2
  %.fca.2.gep87 = getelementptr inbounds %struct.String, ptr %17, i64 0, i32 2
  store i64 %.fca.2.extract86, ptr %.fca.2.gep87, align 8
  %56 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %55)
  call void @_ZN6String4ctorEPc(ptr nonnull %18, ptr nonnull @anon.string.39) #2
  %57 = call %struct.String @_Z6op.mulsRK6String(i16 4, ptr nonnull %18) #2
  %.fca.0.extract76 = extractvalue %struct.String %57, 0
  store ptr %.fca.0.extract76, ptr %19, align 8
  %.fca.1.extract78 = extractvalue %struct.String %57, 1
  %.fca.1.gep79 = getelementptr inbounds %struct.String, ptr %19, i64 0, i32 1
  store i64 %.fca.1.extract78, ptr %.fca.1.gep79, align 8
  %.fca.2.extract80 = extractvalue %struct.String %57, 2
  %.fca.2.gep81 = getelementptr inbounds %struct.String, ptr %19, i64 0, i32 2
  store i64 %.fca.2.extract80, ptr %.fca.2.gep81, align 8
  %58 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %57)
  call void @_ZN6String4ctorEPc(ptr nonnull %20, ptr nonnull @anon.string.10) #2
  %59 = call %struct.String @_Z6op.mulRK6Stringi(ptr nonnull %20, i32 5) #2
  %.fca.0.extract55 = extractvalue %struct.String %59, 0
  %.fca.1.extract57 = extractvalue %struct.String %59, 1
  %.fca.1.gep58 = getelementptr inbounds %struct.String, ptr %s2, i64 0, i32 1
  %.fca.2.extract59 = extractvalue %struct.String %59, 2
  %.fca.2.gep60 = getelementptr inbounds %struct.String, ptr %s2, i64 0, i32 2
  store ptr %.fca.0.extract55, ptr %s2, align 8
  store i64 %.fca.1.extract57, ptr %.fca.1.gep58, align 8
  store i64 %.fca.2.extract59, ptr %.fca.2.gep60, align 8
  %60 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %59)
  call void @_ZN6String4ctorEh(ptr nonnull %21, i8 97) #2
  %61 = call %struct.String @_Z6op.muliRK6String(i32 20, ptr nonnull %21) #2
  %.fca.0.extract49 = extractvalue %struct.String %61, 0
  store ptr %.fca.0.extract49, ptr %22, align 8
  %.fca.1.extract51 = extractvalue %struct.String %61, 1
  %.fca.1.gep52 = getelementptr inbounds %struct.String, ptr %22, i64 0, i32 1
  store i64 %.fca.1.extract51, ptr %.fca.1.gep52, align 8
  %.fca.2.extract53 = extractvalue %struct.String %61, 2
  %.fca.2.gep54 = getelementptr inbounds %struct.String, ptr %22, i64 0, i32 2
  store i64 %.fca.2.extract53, ptr %.fca.2.gep54, align 8
  %62 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %61)
  call void @_ZN6String4ctorEh(ptr nonnull %23, i8 99) #2
  %63 = call %struct.String @_Z6op.muliRK6String(i32 2, ptr nonnull %23) #2
  %.fca.0.extract43 = extractvalue %struct.String %63, 0
  store ptr %.fca.0.extract43, ptr %24, align 8
  %.fca.1.extract45 = extractvalue %struct.String %63, 1
  %.fca.1.gep46 = getelementptr inbounds %struct.String, ptr %24, i64 0, i32 1
  store i64 %.fca.1.extract45, ptr %.fca.1.gep46, align 8
  %.fca.2.extract47 = extractvalue %struct.String %63, 2
  %.fca.2.gep48 = getelementptr inbounds %struct.String, ptr %24, i64 0, i32 2
  store i64 %.fca.2.extract47, ptr %.fca.2.gep48, align 8
  %64 = call %struct.String @_Z6op.mulRK6Stringi(ptr nonnull %24, i32 7) #2
  %.fca.0.extract22 = extractvalue %struct.String %64, 0
  %.fca.1.extract24 = extractvalue %struct.String %64, 1
  %.fca.1.gep25 = getelementptr inbounds %struct.String, ptr %s3, i64 0, i32 1
  %.fca.2.extract26 = extractvalue %struct.String %64, 2
  %.fca.2.gep27 = getelementptr inbounds %struct.String, ptr %s3, i64 0, i32 2
  store ptr %.fca.0.extract22, ptr %s3, align 8
  store i64 %.fca.1.extract24, ptr %.fca.1.gep25, align 8
  store i64 %.fca.2.extract26, ptr %.fca.2.gep27, align 8
  %65 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %64)
  call void @_ZN6String4ctorEPc(ptr nonnull %25, ptr nonnull @anon.string.11) #2
  %66 = call %struct.String @_Z6op.mulRK6Stringi(ptr nonnull %25, i32 1) #2
  %.fca.0.extract = extractvalue %struct.String %66, 0
  store ptr %.fca.0.extract, ptr %26, align 8
  %.fca.1.extract = extractvalue %struct.String %66, 1
  %.fca.1.gep20 = getelementptr inbounds %struct.String, ptr %26, i64 0, i32 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep20, align 8
  %.fca.2.extract = extractvalue %struct.String %66, 2
  %.fca.2.gep21 = getelementptr inbounds %struct.String, ptr %26, i64 0, i32 2
  store i64 %.fca.2.extract, ptr %.fca.2.gep21, align 8
  %67 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %66)
  %68 = call i1 @_Z10isRawEqualPcPc(ptr nonnull @anon.string.30, ptr nonnull @anon.string.31) #2
  %69 = zext i1 %68 to i32
  %70 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.14, i32 %69)
  %71 = call i1 @_Z10isRawEqualPcPc(ptr nonnull @anon.string.36, ptr nonnull @anon.string.33) #2
  %72 = zext i1 %71 to i32
  %73 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.14, i32 %72)
  %74 = call i1 @_Z10isRawEqualPcPc(ptr nonnull @anon.string.36, ptr nonnull @anon.string.36) #2
  %75 = zext i1 %74 to i32
  %76 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.14, i32 %75)
  call void @_ZN6String4ctorEPc(ptr nonnull %27, ptr nonnull @anon.string.30) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %28, ptr nonnull @anon.string.31) #2
  %77 = call i1 @_Z8op.equalRK6StringRK6String(ptr nonnull %27, ptr nonnull %28) #2
  %78 = zext i1 %77 to i32
  %79 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i32 %78)
  call void @_ZN6String4ctorEPc(ptr nonnull %29, ptr nonnull @anon.string.36) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %30, ptr nonnull @anon.string.33) #2
  %80 = call i1 @_Z8op.equalRK6StringRK6String(ptr nonnull %29, ptr nonnull %30) #2
  %81 = zext i1 %80 to i32
  %82 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i32 %81)
  call void @_ZN6String4ctorEPc(ptr nonnull %31, ptr nonnull @anon.string.36) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %32, ptr nonnull @anon.string.36) #2
  %83 = call i1 @_Z8op.equalRK6StringRK6String(ptr nonnull %31, ptr nonnull %32) #2
  %84 = zext i1 %83 to i32
  %85 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.17, i32 %84)
  %86 = call i1 @_Z10isRawEqualPcPc(ptr nonnull @anon.string.30, ptr nonnull @anon.string.31) #2
  %87 = xor i1 %86, true
  %88 = zext i1 %87 to i32
  %89 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.20, i32 %88)
  %90 = call i1 @_Z10isRawEqualPcPc(ptr nonnull @anon.string.36, ptr nonnull @anon.string.33) #2
  %91 = xor i1 %90, true
  %92 = zext i1 %91 to i32
  %93 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.20, i32 %92)
  %94 = call i1 @_Z10isRawEqualPcPc(ptr nonnull @anon.string.36, ptr nonnull @anon.string.36) #2
  %95 = xor i1 %94, true
  %96 = zext i1 %95 to i32
  %97 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.20, i32 %96)
  call void @_ZN6String4ctorEPc(ptr nonnull %33, ptr nonnull @anon.string.30) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %34, ptr nonnull @anon.string.31) #2
  %98 = call i1 @_Z11op.notequalRK6StringRK6String(ptr nonnull %33, ptr nonnull %34) #2
  %99 = zext i1 %98 to i32
  %100 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.23, i32 %99)
  call void @_ZN6String4ctorEPc(ptr nonnull %35, ptr nonnull @anon.string.36) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %36, ptr nonnull @anon.string.33) #2
  %101 = call i1 @_Z11op.notequalRK6StringRK6String(ptr nonnull %35, ptr nonnull %36) #2
  %102 = zext i1 %101 to i32
  %103 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.23, i32 %102)
  call void @_ZN6String4ctorEPc(ptr nonnull %37, ptr nonnull @anon.string.36) #2
  call void @_ZN6String4ctorEPc(ptr nonnull %38, ptr nonnull @anon.string.36) #2
  %104 = call i1 @_Z11op.notequalRK6StringRK6String(ptr nonnull %37, ptr nonnull %38) #2
  %105 = zext i1 %104 to i32
  %106 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.23, i32 %105)
  call void @_ZN6String4ctorEPc(ptr nonnull %s4, ptr nonnull @anon.string.36) #2
  store i8 108, ptr %39, align 1
  call void @_Z12op.plusequalR6StringRKh(ptr nonnull %s4, ptr nonnull %39) #2
  %.fca.0.load11 = load ptr, ptr %s4, align 8
  %.fca.0.insert12 = insertvalue %struct.String poison, ptr %.fca.0.load11, 0
  %.fca.1.gep13 = getelementptr inbounds %struct.String, ptr %s4, i64 0, i32 1
  %.fca.1.load14 = load i64, ptr %.fca.1.gep13, align 8
  %.fca.1.insert15 = insertvalue %struct.String %.fca.0.insert12, i64 %.fca.1.load14, 1
  %.fca.2.gep16 = getelementptr inbounds %struct.String, ptr %s4, i64 0, i32 2
  %.fca.2.load17 = load i64, ptr %.fca.2.gep16, align 8
  %.fca.2.insert18 = insertvalue %struct.String %.fca.1.insert15, i64 %.fca.2.load17, 2
  %107 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %.fca.2.insert18)
  call void @_ZN6String4ctorEPc(ptr nonnull %s5, ptr nonnull @anon.string.39) #2
  store ptr @anon.string.38, ptr %40, align 8
  call void @_Z12op.plusequalR6StringRKPc(ptr nonnull %s5, ptr nonnull %40) #2
  %.fca.0.load2 = load ptr, ptr %s5, align 8
  %.fca.0.insert3 = insertvalue %struct.String poison, ptr %.fca.0.load2, 0
  %.fca.1.gep4 = getelementptr inbounds %struct.String, ptr %s5, i64 0, i32 1
  %.fca.1.load5 = load i64, ptr %.fca.1.gep4, align 8
  %.fca.1.insert6 = insertvalue %struct.String %.fca.0.insert3, i64 %.fca.1.load5, 1
  %.fca.2.gep7 = getelementptr inbounds %struct.String, ptr %s5, i64 0, i32 2
  %.fca.2.load8 = load i64, ptr %.fca.2.gep7, align 8
  %.fca.2.insert9 = insertvalue %struct.String %.fca.1.insert6, i64 %.fca.2.load8, 2
  %108 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %.fca.2.insert9)
  call void @_ZN6String4ctorEPc(ptr nonnull %s6, ptr nonnull @anon.string.39) #2
  call void @_Z11op.mulequalR6Stringi(ptr nonnull %s6, i32 3) #2
  %.fca.0.load = load ptr, ptr %s6, align 8
  %.fca.0.insert = insertvalue %struct.String poison, ptr %.fca.0.load, 0
  %.fca.1.gep = getelementptr inbounds %struct.String, ptr %s6, i64 0, i32 1
  %.fca.1.load = load i64, ptr %.fca.1.gep, align 8
  %.fca.1.insert = insertvalue %struct.String %.fca.0.insert, i64 %.fca.1.load, 1
  %.fca.2.gep = getelementptr inbounds %struct.String, ptr %s6, i64 0, i32 2
  %.fca.2.load = load i64, ptr %.fca.2.gep, align 8
  %.fca.2.insert = insertvalue %struct.String %.fca.1.insert, i64 %.fca.2.load, 2
  %109 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.26, %struct.String %.fca.2.insert)
  call void @_ZN6String4dtorEv(ptr nonnull %37) #2
  call void @_ZN6String4dtorEv(ptr nonnull %38) #2
  call void @_ZN6String4dtorEv(ptr nonnull %36) #2
  call void @_ZN6String4dtorEv(ptr nonnull %33) #2
  call void @_ZN6String4dtorEv(ptr nonnull %34) #2
  call void @_ZN6String4dtorEv(ptr nonnull %31) #2
  call void @_ZN6String4dtorEv(ptr nonnull %10) #2
  call void @_ZN6String4dtorEv(ptr nonnull %15) #2
  call void @_ZN6String4dtorEv(ptr nonnull %24) #2
  call void @_ZN6String4dtorEv(ptr nonnull %12) #2
  call void @_ZN6String4dtorEv(ptr nonnull %7) #2
  call void @_ZN6String4dtorEv(ptr nonnull %5) #2
  call void @_ZN6String4dtorEv(ptr nonnull %35) #2
  call void @_ZN6String4dtorEv(ptr nonnull %2) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s6) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s4) #2
  call void @_ZN6String4dtorEv(ptr nonnull %17) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s5) #2
  call void @_ZN6String4dtorEv(ptr nonnull %3) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s3) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s1) #2
  call void @_ZN6String4dtorEv(ptr nonnull %13) #2
  call void @_ZN6String4dtorEv(ptr nonnull %s2) #2
  call void @_ZN6String4dtorEv(ptr nonnull %18) #2
  call void @_ZN6String4dtorEv(ptr nonnull %23) #2
  call void @_ZN6String4dtorEv(ptr nonnull %21) #2
  call void @_ZN6String4dtorEv(ptr nonnull %28) #2
  call void @_ZN6String4dtorEv(ptr nonnull %22) #2
  call void @_ZN6String4dtorEv(ptr nonnull %19) #2
  call void @_ZN6String4dtorEv(ptr nonnull %26) #2
  call void @_ZN6String4dtorEv(ptr nonnull %27) #2
  call void @_ZN6String4dtorEv(ptr nonnull %9) #2
  call void @_ZN6String4dtorEv(ptr nonnull %30) #2
  call void @_ZN6String4dtorEv(ptr nonnull %29) #2
  call void @_ZN6String4dtorEv(ptr nonnull %32) #2
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @_ZN6String4ctorEPc(ptr, ptr) local_unnamed_addr

declare %struct.String @_Z7op.plusRK6StringRK6String(ptr, ptr) local_unnamed_addr

declare %struct.String @_Z7op.plusRK6StringRKPc(ptr, ptr) local_unnamed_addr

declare %struct.String @_Z6op.mulsRK6String(i16, ptr) local_unnamed_addr

declare %struct.String @_Z6op.mulRK6Stringi(ptr, i32) local_unnamed_addr

declare void @_ZN6String4ctorEh(ptr, i8) local_unnamed_addr

declare %struct.String @_Z6op.muliRK6String(i32, ptr) local_unnamed_addr

declare i1 @_Z10isRawEqualPcPc(ptr, ptr) local_unnamed_addr

declare i1 @_Z8op.equalRK6StringRK6String(ptr, ptr) local_unnamed_addr

declare i1 @_Z11op.notequalRK6StringRK6String(ptr, ptr) local_unnamed_addr

declare void @_Z12op.plusequalR6StringRKh(ptr, ptr) local_unnamed_addr

declare void @_Z12op.plusequalR6StringRKPc(ptr, ptr) local_unnamed_addr

declare void @_Z11op.mulequalR6Stringi(ptr, i32) local_unnamed_addr

declare void @_ZN6String4dtorEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
