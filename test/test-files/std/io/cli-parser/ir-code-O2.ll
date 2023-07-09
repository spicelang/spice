; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.CliParser = type { ptr, ptr, ptr, ptr }

@anon.string.0 = private unnamed_addr constant [13 x i8] c"Test Program\00", align 1
@anon.string.1 = private unnamed_addr constant [30 x i8] c"This is a simple test program\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"v0.1.0\00", align 1
@anon.string.3 = private unnamed_addr constant [37 x i8] c"Copyright (c) Marc Auberer 2021-2023\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) local_unnamed_addr #0 {
  %parser = alloca %struct.CliParser, align 8
  call void @_ZN9CliParser4ctorEPcPc(ptr nonnull %parser, ptr nonnull @anon.string.0, ptr nonnull @anon.string.1) #1
  call void @_ZN9CliParser10setVersionEPc(ptr nonnull %parser, ptr nonnull @anon.string.2) #1
  call void @_ZN9CliParser9setFooterEPc(ptr nonnull %parser, ptr nonnull @anon.string.3) #1
  %3 = call i32 @_ZN9CliParser5parseEjPPc(ptr nonnull %parser, i32 %0, ptr %1) #1
  ret i32 %3
}

declare void @_ZN9CliParser4ctorEPcPc(ptr, ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser10setVersionEPc(ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser9setFooterEPc(ptr, ptr) local_unnamed_addr

declare i32 @_ZN9CliParser5parseEjPPc(ptr, i32, ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }
