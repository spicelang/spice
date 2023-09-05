; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.CliParser = type { %struct.CliSubcommand.11 }
%struct.CliSubcommand.11 = type { ptr, ptr, ptr, ptr, ptr, { ptr, ptr }, %struct.Vector, %struct.Vector.0, %struct.Vector.1, %struct.Vector.2, i1 }
%struct.Vector = type { ptr, i64, i64 }
%struct.Vector.0 = type { ptr, i64, i64 }
%struct.Vector.1 = type { ptr, i64, i64 }
%struct.Vector.2 = type { ptr, i64, i64 }
%struct.CliOptions = type { ptr }

@anon.string.0 = private unnamed_addr constant [13 x i8] c"Test Program\00", align 1
@anon.string.1 = private unnamed_addr constant [30 x i8] c"This is a simple test program\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"v0.1.0\00", align 1
@anon.string.3 = private unnamed_addr constant [37 x i8] c"Copyright (c) Marc Auberer 2021-2023\00", align 1
@anon.string.5 = private unnamed_addr constant [6 x i8] c"greet\00", align 1
@anon.string.6 = private unnamed_addr constant [14 x i8] c"Greet someone\00", align 1
@anon.string.7 = private unnamed_addr constant [7 x i8] c"--name\00", align 1
@anon.string.8 = private unnamed_addr constant [28 x i8] c"Name of the person to greet\00", align 1
@anon.string.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@printf.str.0 = private unnamed_addr constant [11 x i8] c"Hello %s!\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) local_unnamed_addr #0 {
  %parser = alloca %struct.CliParser, align 8
  %options = alloca %struct.CliOptions, align 8
  call void @_ZN9CliParser4ctorEPcPc(ptr nonnull %parser, ptr nonnull @anon.string.0, ptr nonnull @anon.string.1) #2
  call void @_ZN9CliParser10setVersionEPc(ptr nonnull %parser, ptr nonnull @anon.string.2) #2
  call void @_ZN9CliParser9setFooterEPc(ptr nonnull %parser, ptr nonnull @anon.string.3) #2
  store ptr @anon.string.9, ptr %options, align 8
  %3 = call ptr @_ZN9CliParser13addSubcommandEPcPc(ptr nonnull %parser, ptr nonnull @anon.string.5, ptr nonnull @anon.string.6) #2
  call void @_ZN13CliSubcommand9addOptionEPcRPcPc(ptr %3, ptr nonnull @anon.string.7, ptr nonnull %options, ptr nonnull @anon.string.8) #2
  %4 = call i32 @_ZN9CliParser5parseEjPPc(ptr nonnull %parser, i32 %0, ptr %1) #2
  %5 = load ptr, ptr %options, align 8
  %6 = call i1 @_Z10isRawEqualPcPc(ptr %5, ptr nonnull @anon.string.9) #2
  br i1 %6, label %if.exit.L23, label %if.then.L23

if.then.L23:                                      ; preds = %2
  %7 = load ptr, ptr %options, align 8
  %8 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, ptr %7)
  br label %if.exit.L23

if.exit.L23:                                      ; preds = %if.then.L23, %2
  ret i32 0
}

declare void @_ZN9CliParser4ctorEPcPc(ptr, ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser10setVersionEPc(ptr, ptr) local_unnamed_addr

declare void @_ZN9CliParser9setFooterEPc(ptr, ptr) local_unnamed_addr

declare ptr @_ZN9CliParser13addSubcommandEPcPc(ptr, ptr, ptr) local_unnamed_addr

declare void @_ZN13CliSubcommand9addOptionEPcRPcPc(ptr, ptr, ptr, ptr) local_unnamed_addr

declare i32 @_ZN9CliParser5parseEjPPc(ptr, i32, ptr) local_unnamed_addr

declare i1 @_Z10isRawEqualPcPc(ptr, ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }
