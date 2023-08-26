; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.CliParser = type { ptr, %struct.CliSubcommand.15 }
%struct.CliSubcommand.15 = type { ptr, ptr, ptr, ptr, { ptr, ptr }, %struct.Vector, %struct.Vector.0, %struct.Vector.1, %struct.Vector.2, %struct.Vector.3, %struct.Vector.4, i1 }
%struct.Vector = type { ptr, i64, i64 }
%struct.Vector.0 = type { ptr, i64, i64 }
%struct.Vector.1 = type { ptr, i64, i64 }
%struct.Vector.2 = type { ptr, i64, i64 }
%struct.Vector.3 = type { ptr, i64, i64 }
%struct.Vector.4 = type { ptr, i64, i64 }
%struct.CliOptions = type { i1 }

@printf.str.0 = private unnamed_addr constant [31 x i8] c"Callback called with value %d\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [13 x i8] c"Test Program\00", align 1
@anon.string.1 = private unnamed_addr constant [30 x i8] c"This is a simple test program\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"v0.1.0\00", align 1
@anon.string.3 = private unnamed_addr constant [37 x i8] c"Copyright (c) Marc Auberer 2021-2023\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c"--hi\00", align 1
@anon.string.5 = private unnamed_addr constant [19 x i8] c"Say hi to the user\00", align 1
@anon.string.6 = private unnamed_addr constant [11 x i8] c"--callback\00", align 1
@anon.string.8 = private unnamed_addr constant [4 x i8] c"-cb\00", align 1
@printf.str.1 = private unnamed_addr constant [25 x i8] c"CB called with value %d\0A\00", align 1
@anon.string.9 = private unnamed_addr constant [25 x i8] c"Call a callback function\00", align 1
@str = private unnamed_addr constant [4 x i8] c"Hi!\00", align 1

; Function Attrs: nofree nounwind
define private void @_Z8callbackRb(ptr nocapture readonly %0) #0 {
  %2 = load i1, ptr %0, align 1
  %3 = zext i1 %2 to i32
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 %3)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) local_unnamed_addr #1 {
  %parser = alloca %struct.CliParser, align 8
  %options = alloca %struct.CliOptions, align 8
  call void @_ZN9CliParser4ctorEPcPc(ptr nonnull %parser, ptr nonnull @anon.string.0, ptr nonnull @anon.string.1) #2
  call void @_ZN9CliParser10setVersionEPc(ptr nonnull %parser, ptr nonnull @anon.string.2) #2
  call void @_ZN9CliParser9setFooterEPc(ptr nonnull %parser, ptr nonnull @anon.string.3) #2
  store i1 false, ptr %options, align 8
  call void @_ZN9CliParser7addFlagEPcRbPc(ptr nonnull %parser, ptr nonnull @anon.string.4, ptr nonnull %options, ptr nonnull @anon.string.5) #2
  call void @_ZN9CliParser7addFlagEPcPFvRbEPc(ptr nonnull %parser, ptr nonnull @anon.string.6, { ptr, ptr } { ptr @_Z8callbackRb, ptr undef }, ptr nonnull @anon.string.9) #2
  call void @_ZN9CliParser7addFlagEPcPFvRbEPc(ptr nonnull %parser, ptr nonnull @anon.string.8, { ptr, ptr } { ptr @_Z15lambda.L19C27.0Rb, ptr undef }, ptr nonnull @anon.string.9) #2
  %3 = call i32 @_ZN9CliParser5parseEjPPc(ptr nonnull %parser, i32 %0, ptr %1) #2
  %4 = load i1, ptr %options, align 8
  br i1 %4, label %if.then.L26, label %if.exit.L26

if.then.L26:                                      ; preds = %2
  %puts = call i32 @puts(ptr nonnull dereferenceable(1) @str)
  br label %if.exit.L26

if.exit.L26:                                      ; preds = %if.then.L26, %2
  ret i32 0
}

declare void @_ZN9CliParser4ctorEPcPc(ptr, ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser10setVersionEPc(ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser9setFooterEPc(ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser7addFlagEPcRbPc(ptr, ptr, ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser7addFlagEPcPFvRbEPc(ptr, ptr, { ptr, ptr }, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
define private void @_Z15lambda.L19C27.0Rb(ptr nocapture readonly %0) #0 {
  %2 = load i1, ptr %0, align 1
  %3 = zext i1 %2 to i32
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %3)
  ret void
}

declare i32 @_ZN9CliParser5parseEjPPc(ptr, i32, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nounwind }
