
class ReportParser:
    def __init__(self):
        pass

    """Reads report's file content from provided path"""
    def read_content(self, path):
        file_handler = open(path, 'r')
        content = file_handler.read()
        return content

    """Finds MSE error line and returns value"""
    def get_mse(self, content):
        for item in content.split("\n"):
            if "MSE" in item:
                print item