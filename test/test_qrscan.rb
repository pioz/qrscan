# frozen_string_literal: true

require "test_helper"

class TestQrscan < Minitest::Test
  def test_scan
    value = Qrscan.scan("test/fixtures/files/image.jpg")

    assert_equal "13", value
  end

  def test_scan_without_qr_code
    value = Qrscan.scan("test/fixtures/files/image_no_code.jpg")

    assert_nil value
  end

  def test_raise_error_if_invalid_path
    error = assert_raises Qrscan::QrscanError do
      Qrscan.scan("test/fixtures/files/file_not_found.jpg")
    end

    assert_equal "Error loading image", error.message
  end
end
